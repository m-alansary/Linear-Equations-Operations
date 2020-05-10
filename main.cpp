#include <iostream>
#include <string>
#include <sstream>

#define LEN 100

using namespace std;

string alphabetic = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
string integers = "0123456789.";
string operators = "+-=";

int alphLen = alphabetic.length();
int intLen = integers.length();
int opLen = operators.length();
int vars_num = 0;
int numOfEqn;

void VarMat(string equation, string (&varMat)[LEN]);
void CoefMat(string equation, float (&coefMat)[LEN][LEN], string varMat[LEN], int eqnNum);
void AbsMat(string equation, float (&absMat)[LEN], int eqnNum);
void Normalization(string (&equation)[LEN], string varMat[LEN], float coefMat[LEN][LEN], float absMat[LEN], int eqnNum, int numOfEqn);
void sorting(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], int numOfEqn);

string add(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], int eqn1, int eqn2);
string sub(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], int eqn1, int eqn2);
string substitute (string (&varMat)[LEN], float coefMat[LEN][LEN], float absMat[LEN], int eqn1, int eqn2, string var);
void Det(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], float (&D)[LEN][LEN], string var);
void Det2(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], float (&D)[LEN][LEN], string var, int row, int col);
float DetValue(float (&D)[LEN][LEN],string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], string var, int vars_num);
void solve(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN]);


string FloatToString(float f);
float StringToFloat(string s);
int StringToInt(string s);
bool intCheck (char s);
bool alphCheck (char c);
bool opCheck (char c);
float D[LEN][LEN];

int recTimer = 0;

int main()
{
    vars_num = 0;                   // a counter of variables starting from 0
    string equation[LEN];           // an array to store equations
    string equation2[LEN];          // an array to store equations with a "+" at the end to work on
    string varMat[LEN];             // an array to store variables of the equations
    float coefMat[LEN][LEN];        // an array to store the coefficient of the variables of the equations
    float absMat[LEN];              // an array to store the absolute value of the equations
    int i;                          // for loops counter
    int j;                          // for loops counter
    cin >> numOfEqn;                // taking the number of equations from the user
    // initializing the arrays with 0 for float and "" for string
    for (i=0;i<LEN;i++)
    {
        for(j=0;j<LEN;j++)
        {
            coefMat[i][j]=0;
            D[i][j] = 0;
        }
        absMat[i]=0;
        varMat[i]="";
    }
    // Saving each equation parameters in arrays and normalizing and sorting the variables  and it's coefficients
    for (i=0;i<numOfEqn;i++)
    {
        string temp;
        cin >> temp;
        equation[i] = temp;
        equation2[i] = temp + "+";
        VarMat(equation2[i], varMat);
        CoefMat(equation2[i], coefMat, varMat, i);
        AbsMat(equation2[i], absMat, i);
        sorting(varMat, coefMat, numOfEqn);
        Normalization(equation, varMat, coefMat, absMat, i,numOfEqn);
    }
    sorting(varMat, coefMat, numOfEqn);
    // Taking the required operation from the user
    while (1)
    {
        string operation;
        getline(cin,operation);
        if (operation == "num_vars")
            cout << vars_num << endl;
        else if (operation.substr(0,9) == "equation ")
        {
            int intlen = operation.length()-8;
            int eqnNum = StringToFloat(operation.substr(9,intlen));
            cout << equation[eqnNum-1] << endl;
        }
        else if (operation.substr(0,7) == "column ")
        {
            int intlen = operation.length()-6;
            string var = operation.substr(7,intlen);
            for (i=0;i<numOfEqn;i++)
            {
                if (varMat[i] == var)
                    break;
            }
            for (j=0;j<numOfEqn;j++)
            {
                cout << coefMat[j][i] << endl;
            }
        }
        else if (operation.substr(0,4) == "add ")
        {
            int eqn1;
            int eqn2;
            if(intCheck(operation[4]) && intCheck(operation[5]) && intCheck(operation[6]) && operation[7] == ' ')
            {
                eqn1 = StringToInt(operation.substr(4,3)) - 1;
                eqn2 = StringToInt(operation.substr(8,operation.length()-8)) - 1;
            }
            else if(intCheck(operation[4]) && intCheck(operation[5]) && operation[6] == ' ')
            {
                eqn1 = StringToInt(operation.substr(4,2)) - 1;
                eqn2 = StringToInt(operation.substr(7,operation.length()-7)) - 1;
            }
            else if(intCheck(operation[4]) && operation[5] == ' ')
            {
                eqn1 = StringToInt(operation.substr(4,1)) - 1;
                eqn2 = StringToInt(operation.substr(6,operation.length()-6)) - 1;
            }
            string result = add(varMat, coefMat, absMat, eqn1, eqn2);
            cout << result << endl;
        }
        else if (operation.substr(0,9) == "subtract ")
        {
            int eqn1;
            int eqn2;
            if(intCheck(operation[9]) && intCheck(operation[10]) && intCheck(operation[11]) && operation[12] == ' ')
            {
                eqn1 = StringToInt(operation.substr(9,3)) - 1;
                eqn2 = StringToInt(operation.substr(13,operation.length()-13)) - 1;
            }
            else if(intCheck(operation[9]) && intCheck(operation[10]) && operation[11] == ' ')
            {
                eqn1 = StringToInt(operation.substr(9,2)) - 1;
                eqn2 = StringToInt(operation.substr(12,operation.length()-12)) - 1;
            }
            else if(intCheck(operation[9]) && operation[10] == ' ')
            {
                eqn1 = StringToInt(operation.substr(9,1)) - 1;
                eqn2 = StringToInt(operation.substr(11,operation.length()-11)) - 1;
            }
            string result = sub(varMat, coefMat, absMat, eqn1, eqn2);
            cout << result << endl;
        }
        else if (operation.substr(0,11) == "substitute ")
        {
            operation += " ";
            string var;
            int eqn1;
            int eqn2;
            string s;
            bool flag =false;
            for (i=11;i<operation.length()+1;i++)
            {
                char c = operation[i];
                if(alphCheck(c))
                {
                    s += c;
                    flag = true;
                }
                else if (intCheck(c) && flag)
                {
                    s+=c;
                }
                else if (intCheck(c))
                {
                    s+=c;
                }
                else if (c == ' ')
                {
                    if (flag)
                    {
                        flag = false;
                        var = s;
                        s = "";
                    }
                    else
                    {
                        if (i < operation.length() - 1)
                        {
                            eqn1 = StringToFloat(s) - 1;
                            s = "";
                        }
                        else
                        {
                            eqn2 = StringToFloat(s) - 1;
                            s = "";
                        }
                    }
                }
            }
            string result = substitute(varMat, coefMat, absMat, eqn1, eqn2,var);
            cout << result << endl;

        }
        else if(operation == "D_value")
        {
            Det(varMat, coefMat, absMat, D, "");
            int temps = vars_num;
            float value = DetValue(D, varMat, coefMat, absMat, "", temps);
            cout << value << endl;
        }
        else if(operation[0] == 'D')
        {
            if(operation.length() == 1)
                Det(varMat, coefMat, absMat, D, "");
            else
                Det(varMat, coefMat, absMat, D, operation.substr(2,operation.length() - 2));
            for(i=0;i<numOfEqn;i++)
            {
                for(j=0;j<vars_num;j++)
                {
                    cout << D[i][j];
                    if(j != vars_num - 1)
                        cout << "\t";
                }
                cout << endl;
            }
        }
        else if(operation == "solve")
        {
            solve(varMat, coefMat, absMat);
        }
    }
    return 0;
}

void VarMat(string equation, string (&varMat)[LEN])
{
    //this function takes an equation and the variables matrix and add the equation's new variables to it's matrix
    string t="";
    int equLen = equation.length();
    int i;
    for (i=0;i<equLen;i++)
    {
        char c = equation[i];
        if (alphCheck(c))
            t += c;
        else if (intCheck(c) && i != 0 && t != "")
            t += c;
        if ((opCheck(c) && t != "") || (i == equLen-1 && t != ""))
        {
            bool f=true;
            int k;
            for (k=0;k<LEN;k++)
            {
                if (t == varMat[k])
                    f=false;
            }
            if(f)
            {
                varMat[vars_num] = t;
                vars_num++;
                t = "";
            }
            t="";
        }
    }
}

void CoefMat(string equation, float (&coefMat)[LEN][LEN], string varMat[LEN], int eqnNum)
{
    //this function takes an equation, coefficient matrix ,and the variables matrix and add the equation's coefficients to it's matrix
    string coef = "";
    string var = "";
    int equLen = equation.length();
    int i;
    bool equalOp = false;
    char op = '+';
    for (i=0;i<equLen;i++)
    {
        char c = equation[i];
        if (alphCheck(c))
            var += c;
        else if (intCheck(c) && i != 0 && var != "")
            var += c;
        else if (intCheck(c))
            coef += c;
        if ((opCheck(c) && var != "" ))
        {
            int j;
            if (op == '=')
                equalOp = true;
            for (j=0;j<LEN;j++)
            {
                if (var == varMat[j])
                    break;
            }
            if (coef == "")
                coef = "1";
            float coef2 = StringToFloat(coef);
            if (op == '-' && !equalOp)
                coef2 = -coef2;
            else if ((op == '+' || op == '=')  && equalOp)
                coef2 = -coef2;
            if (var != "")
                coefMat[eqnNum][j] += coef2;
            coef = "";
            var = "";
            op = c;
        }
        else if (opCheck(c))
        {
            if (op == '=')
                equalOp = true;
            coef = "";
            var = "";
            op = c;
        }
    }
}

void AbsMat(string equation, float (&absMat)[LEN], int eqnNum)
{
    //this function takes an equation and the absolute values matrix and add the equation's absolute value to it's matrix
    string abs = "";
    string var = "";
    int equLen = equation.length();
    int i;
    bool equalOp = false;
    char op = '+';
    for (i=0;i<equLen;i++)
    {
        char c = equation[i];
        if (alphCheck(c))
            var += c;
        else if (intCheck(c) && i != 0 && var != "")
            var += c;
        else if (intCheck(c))
            abs += c;
        if ((opCheck(c) && abs != ""))
        {
            if (op == '=')
                equalOp = true;
            float abs2 = StringToFloat(abs);
            if (op == '+' && !equalOp)
                abs2 = -abs2;
            else if (op == '-'&& equalOp)
                abs2 = -abs2;
            if (var == "")
                absMat[eqnNum] += abs2;
            abs = "";
            var = "";
            op = c;
        }
        else if (opCheck(c))
        {
            if (op == '=')
                equalOp = true;
            abs = "";
            var = "";
            op = c;
        }
    }
}

void Normalization(string (&equation)[LEN], string varMat[LEN], float coefMat[LEN][LEN], float absMat[LEN], int eqnNum, int numOfEqn)
{
    //this function takes an equation, the variables, coefficients, and absolute matrices and normalize the equation on the proper form
    string result = "";
    string coef;
    int i;
    for (i=0;i<=numOfEqn;i++)
    {
        if (coefMat[eqnNum][i] != 0)
        {
            if (coefMat[eqnNum][i] < 0)
                coef = FloatToString(coefMat[eqnNum][i]);
            else if (coefMat[eqnNum][i] > 0 && result == "")
                coef = FloatToString(coefMat[eqnNum][i]);
            else if (coefMat[eqnNum][i] > 0)
                coef = "+" + FloatToString(coefMat[eqnNum][i]);
            result = result + coef + varMat[i];
        }
    }
    if (result == "")
        result += "0";
    result = result + "=" + FloatToString(absMat[eqnNum]);
    equation[eqnNum] = result;
}

void sorting(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], int numOfEqn)
{
    // this function sort the variable and it's coefficients in there matrices
    int i;
    int j;
    int k;
    string s;
    for (i=0;i<vars_num;i++)
    {
        for (j=0;j<=vars_num;j++)
        {
            if (varMat[j] > varMat[i])
            {
                string t = varMat[i];
                varMat[i] = varMat[j];
                varMat[j] = t;
                for (k=0;k<numOfEqn;k++)
                {
                    float temp = coefMat[k][i];
                    coefMat[k][i] = coefMat[k][j];
                    coefMat[k][j] = temp;
                }
            }
        }

    }
}

string add(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], int eqn1, int eqn2)
{
    // this function add 2 equations to each other
    float coef,abs;
    int i;
    string result = "";
    for (i=0;i<vars_num;i++)
        {
            coef = coefMat[eqn1][i] + coefMat[eqn2][i];
            if(coef != 0)
            {
                if (result == "")
                    result += FloatToString(coef) + varMat[i];
                else if (coef>0)
                    result += "+" + FloatToString(coef) + varMat[i];
                else if (coef<0)
                    result += FloatToString(coef) + varMat[i];
            }
        }
    if (result == "")
        result += "0" ;
    abs = absMat[eqn1] + absMat[eqn2];
    result += "=" + FloatToString(abs);
    return result;
}

string sub(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], int eqn1, int eqn2)
{
    // this function subtract two equations from each other
    float coef,abs;
    int i;
    string result = "";
    for (i=0;i<vars_num;i++)
        {
            coef = coefMat[eqn1][i] - coefMat[eqn2][i];
            if (coef != 0)
            {
                if (result == "")
                    result += FloatToString(coef) + varMat[i];
                else if (coef>=0)
                    result += "+" + FloatToString(coef) + varMat[i];
                else if (coef<0)
                    result += FloatToString(coef) + varMat[i];
            }

        }
    if (result == "")
        result += "0";
    abs = absMat[eqn1] - absMat[eqn2];
    result += "=" + FloatToString(abs);
    return result;
}

string substitute(string (&varMat)[LEN], float coefMat[LEN][LEN], float absMat[LEN], int eqn1, int eqn2, string var)
{
    // this function substitute a variable from equation to another
    float coef[LEN][LEN];
    float abs[LEN];
    int i;
    int varIndex;
    for(i=0;i<vars_num;i++)
    {
        coef[eqn1][i] = coefMat[eqn1][i];
        coef[eqn2][i] = coefMat[eqn2][i];
        if (var == varMat[i])
            varIndex = i;
    }
    float temp = coef[eqn2][varIndex];
    float temp2 = coef[eqn1][varIndex];
    for(i=0;i<vars_num;i++)
    {
        coef[eqn1][i] = (coef[eqn1][i] * temp) / temp2;
    }
    abs[eqn1] = (absMat[eqn1] * temp) / temp2;
    abs[eqn2] = absMat[eqn2];
    string result = sub(varMat, coef, abs, eqn1, eqn2);
    return result;
}

void Det(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], float (&D)[LEN][LEN], string var)
{
    // this function get the matrix of the entered variable from the equations it "" entered it gets the coefficient matrix
    int i,j;
    if (var == "")
    {
        for(i=0;i<vars_num;i++)
        {
            for (j=0;j<vars_num;j++)
                D[i][j] = coefMat[i][j];
        }
    }
    else
    {
        for(i=0;i<vars_num;i++)
        {
            for (j=0;j<vars_num;j++)
            {
                if (var == varMat[j])
                    D[i][j] = absMat[i];
                else
                    D[i][j] = coefMat[i][j];
            }
        }
    }
}

void Det2(string (&varMat)[LEN], float D2[LEN][LEN], float (&absMat)[LEN], float (&D)[LEN][LEN], string var, int row, int col, int vars_num2)
{
    // this another version of Det function which is used for the recursive solution
    int i,j;
    int col2 = col;
    if (var == "")
    {

        for(i=0;i<vars_num2;i++)
        {
            for (j=0;j<vars_num2;j++)
            {
                if(j > col2 && i != 0)
                    D[i-1][j-1] = D2[i][j];
                else if (j < col2 && i != 0)
                    D[i-1][j] = D2[i][j];
            }
        }
    }
    else
    {
        for(i=0;i<vars_num;i++)
        {
            for (j=0;j<vars_num;j++)
            {
                if (var == varMat[j])
                {
                    if(j > col2 && i != 0)
                        D[i-1][j-1] = D2[i][j];
                    else if (j < col2 && i != 0)
                        D[i-1][j] = D2[i][j];
                }
                else
                {
                    if(j > col2 && i != 0)
                        D[i-1][j-1] = D2[i][j];
                    else if (j < col2 && i != 0)
                        D[i-1][j] = D2[i][j];
                }
            }
            row++;
        }
    }
}

float DetValue(float (&D)[LEN][LEN],string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN], string var, int vars_num2)
{
    // this function calculate the determinant value and returns it
    recTimer ++;
    int i,j,k,l;
    float result = 0;
    if(vars_num2 == 2)
    {
        return D[0][0]*D[1][1] - D[1][0]*D[0][1];
    }
    else if (vars_num2 == 1)
        return coefMat[0][0];
    else
    {
        for(j=0;j<vars_num2;j++)
        {
            float D2[LEN][LEN];
            for (k=0;k<LEN;k++)
                for (l=0;l<LEN;l++)
                    D2[k][l] = 0;
            Det2(varMat, coefMat, absMat, D2, var, 0, j, vars_num2);
            if (j%2 == 0)
                result += D[0][j] * DetValue(D2, varMat, D2, absMat, var, vars_num2 - 1);
            else
                result -= D[0][j] * DetValue(D2, varMat, D2, absMat, var, vars_num2 - 1);
        }
    }
    return result;
}

void solve(string (&varMat)[LEN], float (&coefMat)[LEN][LEN], float (&absMat)[LEN])
{
    // this function print the solution of the equations
    int i, j, k;
    float D[LEN][LEN];
    for (i=0;i<LEN;i++)
        for (j=0;j<LEN;j++)
            D[i][j] = 0;
    Det(varMat, coefMat, absMat, D, "");
    float Dvalue = DetValue(D, varMat, coefMat, absMat, "", vars_num);
    if (numOfEqn != vars_num)
        cout << "No Solution" << endl;
    else if (Dvalue == 0)
        cout << "No Solution" << endl;
    else
    {
        for (j=0;j<vars_num;j++)
        {
            float D2[LEN][LEN];
            for (i=0;i<LEN;i++)
                for (k=0;k<LEN;k++)
                    D2[i][j] = 0;
            Det(varMat, coefMat, absMat, D2, varMat[j]);
            float D2Value = DetValue(D2, varMat, D2, absMat, varMat[j], vars_num);
            float result = D2Value/Dvalue;
            if (result == -0)
                result = 0;
            cout << varMat[j] << "=" << result << endl;
        }
    }
}

string FloatToString(float f)
{
    // this function changes a float number into a string and returns the string
    string str;
    ostringstream ss;
    ss << f;
    str = ss.str();
    return str;
}

float StringToFloat(string s)
{
    // this function changes a string into a float number and returns it's value
    float x;
    istringstream ( s ) >> x;
    return x;
}

int StringToInt(string s)
{
    // this function changes a string into an integer number and returns it's value
    int x;
    istringstream ( s ) >> x;
    return x;
}

bool intCheck (char c)
{
    // this function checks if a given character is an integer type of a point or not
    int i;
    for (i=0;i<intLen;i++)
    {
        if (c == integers[i])
            return true;
    }
    return false;
}

bool alphCheck (char c)
{
    // this function checks if a given character is an alphabetic letter or not
    int i;
    for (i=0;i<alphLen;i++)
    {
        if (c == alphabetic[i])
            return true;
    }
    return false;
}

bool opCheck (char c)
{
    // this function checks if a given character is a operator "+, -, or =" or not
    int i;
    for (i=0;i<opLen;i++)
    {
        if (c == operators[i])
            return true;
    }
    return false;
}


