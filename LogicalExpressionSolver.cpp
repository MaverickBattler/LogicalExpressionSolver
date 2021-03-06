#include "pch.h"
#include  <string>
#include  <stdio.h>
#include  <conio.h>
#include  <iostream>
#include  <fstream>
#include  <iomanip>
#include  <vector>
#include  <time.h>
#include  <list>
#include  <stack>
#include  <Windows.h>

using namespace std;


/*struct Node {
	char* name;
	char* lec;
	//string name;
	//string lec;//выражение
	bool cnst;//1 или 0
	char var;//Переменная
	Node* parent;
	Node* childrenLeft;
	Node* childrenRight;
};*/


int symboltype(char symbol)
{
	switch (symbol)
	{
	case '~': return 1;//отрицание
	case '(': return 2;//открывающаяся скобка
	case ')': return 3;//закрывающаяся скобка
	case '+': return 4;//бинарная операция
	case '*': return 4;
	case '^': return 4;
	case '@': return 4;
	case '#': return 4;
	case '0': return 5;//константа
	case '1': return 5;
	default: return 6;//переменная
	}
}

int getOperOrd(char _operator)//Возвращает порядок вычисления оператора
{
	switch (_operator)
	{
	case '~'://Отрицание
		return 1;
	case '*'://И(конъюнкция)
		return 2;
	case '+'://Или(дизъюнкция)
		return 3;
	case '^'://Исключающее или
		return 4;
	case '@'://Импликация
		return 5;
	case '#'://Эквивалентность
		return 6;
	case '(':
		return 7;
	case ')':
		return 8;
	default:
		return 0;
	}
}

string exprToOpz(string expr)
{
	string opz;
	stack<char> st;
	int exprsize = expr.length();
	for (int i = 0; i < exprsize; i++)
	{
		switch (symboltype(expr[i]))
		{
		case 1:
			st.push(expr[i]);
			break;
		case 2:
			st.push(expr[i]);
			break;
		case 3:
			while (symboltype(st.top()) != 2)
			{
				opz += st.top();
				st.pop();
			}
			st.pop();
			break;
		case 4:
			if (!st.empty())
				while ((symboltype(st.top()) == 1) || (getOperOrd(st.top()) < getOperOrd(expr[i])) || ((getOperOrd(st.top()) == getOperOrd(expr[i]))))
				{
					opz += st.top();
					st.pop();
					if (st.empty())
						break;
				}
			st.push(expr[i]);
			break;
		case 5:
			opz += expr[i];
			break;
		case 6:
			opz += expr[i];
			break;
		}
	}
	while (!st.empty())
	{
		opz += st.top();
		st.pop();
	}
	return opz;
}

int correctnessOfExpression(string expression)
{
	int ob = 0, cb = 0;
	if (expression.length() == 0)
		return 0;
	int i = 0;
	for (i = 0; i < expression.length(); i++)
	{
		if (i == 0)//Первый символ
		{
			if ((symboltype(expression[i]) == 3) || (symboltype(expression[i]) == 4))
				return 0;//ошибка
			else if (symboltype(expression[i]) == 6)
			{
				if (!(((expression[i] >= 'A') && (expression[i] <= 'Z')) || ((expression[i] >= 'a') && (expression[i] <= 'z'))))//Если это не символ английского алфавита
					return 0;
			}
			if (symboltype(expression[i]) == 2)
				ob++;
		}
		else if ((symboltype(expression[i - 1]) == 1) || (symboltype(expression[i - 1]) == 2) || (symboltype(expression[i - 1]) == 4))
		{//предыдущий символ - отрицание, открывающая скобка, бинарная операция
			if ((symboltype(expression[i]) == 3) || (symboltype(expression[i]) == 4))
				return 0;//ошибка
			else if (symboltype(expression[i]) == 6)
			{
				if (!(((expression[i] >= 'A') && (expression[i] <= 'Z')) || ((expression[i] >= 'a') && (expression[i] <= 'z'))))//Если это не символ английского алфавита
					return 0;
			}
			if (symboltype(expression[i]) == 2)
				ob++;
		}
		else if ((symboltype(expression[i - 1]) == 3) || (symboltype(expression[i - 1]) == 5) || (symboltype(expression[i - 1]) == 6))//Предыдущий символ - закрывающая скобка, константа или переменная
		{
			if ((symboltype(expression[i]) == 1) || (symboltype(expression[i]) == 2) || (symboltype(expression[i]) == 5) || ((symboltype(expression[i]) == 6)))
				return 0;
			if (symboltype(expression[i]) == 3)
				cb++;
		}
	}
	if (i != 1)
		if ((symboltype(expression[i - 1]) == 1) || (symboltype(expression[i - 1]) == 2) || (symboltype(expression[i - 1]) == 4))
			return 0;
	if (ob == cb)
		return 1;
	else
		return 0;
}

int correctnessOfFormula(string formula)
{
	int ob = 0, cb = 0;
	int equivsignamt = 0;
	int symbolafterequiv = 0;
	if (formula.length() == 0)
		return 0;
	int i = 0;
	for (i = 0; i < formula.length(); i++)
	{
		//cout << "formula[i] = " << formula[i] << endl;
		if (i == 0)//Первый символ
		{
			if ((symboltype(formula[i]) == 3) || (symboltype(formula[i]) == 4))
				return 0;//ошибка
			else if (symboltype(formula[i]) == 6)
			{
				if (!(((formula[i] >= 'A') && (formula[i] <= 'Z')) || ((formula[i] >= 'a') && (formula[i] <= 'z'))))//Если это не символ английского алфавита
					return 0;
				else if (equivsignamt)
					symbolafterequiv++;
			}
			if (symboltype(formula[i]) == 5)
				if (equivsignamt)
					symbolafterequiv++;
			if (symboltype(formula[i]) == 2)
				ob++;
		}
		else if ((symboltype(formula[i - 1]) == 1) || (symboltype(formula[i - 1]) == 2) || (symboltype(formula[i - 1]) == 4)||(formula[i-1]=='='))
		{//предыдущий символ - отрицание, открывающая скобка, бинарная операция или знак равно
			if ((symboltype(formula[i]) == 3) || (symboltype(formula[i]) == 4))
				return 0;//ошибка
			else if (symboltype(formula[i]) == 6)
			{
				if (!(((formula[i] >= 'A') && (formula[i] <= 'Z')) || ((formula[i] >= 'a') && (formula[i] <= 'z'))))//Если это не символ английского алфавита
					return 0;
				else if (equivsignamt)
					symbolafterequiv++;
			}
			if (symboltype(formula[i]) == 5)
				if (equivsignamt)
					symbolafterequiv++;
			if (symboltype(formula[i]) == 2)
				ob++;
		}
		else if ((symboltype(formula[i - 1]) == 3) || (symboltype(formula[i - 1]) == 5) || (symboltype(formula[i - 1]) == 6))//Предыдущий символ - закрывающая скобка, константа или переменная
		{
			if ((symboltype(formula[i]) == 1) || (symboltype(formula[i]) == 2) || (symboltype(formula[i]) == 5) || ((symboltype(formula[i]) == 6)&&(formula[i]!='=')))
				return 0;
			if (symboltype(formula[i]) == 3)
				cb++;
			if (formula[i] == '=')
			{
				equivsignamt++;
			}
		}
	}
	if (i != 1)
		if ((symboltype(formula[i - 1]) == 1) || (symboltype(formula[i - 1]) == 2) || (symboltype(formula[i - 1]) == 4) || (formula[i - 1] == '='))
			return 0;
	if ((ob == cb)&&(equivsignamt==1)&&(symbolafterequiv))
		return 1;
	else
		return 0;
}

int checkFormulaVariables(string formula, string func)
{//Проверка, есть ли переменные формулы в функции
	for (int i = 0; i < formula.length(); i++)
		if (symboltype(formula[i]) == 6)
			for (int j = 0; j < func.length(); j++)
				if (func[j] == formula[i])
					return 0;
	return 1;
}

void EnterFromFile(string &func, string &formula)
{
	//Ввод из файла
	ifstream fin("example.txt");
	if (!fin.is_open()) { cerr << "Can't open input file!" << endl;
	cout << "\nPress enter to exit the program." << endl;
	getchar();
	exit(1); }
	fin >> func >> formula;
}

void skipBracketsPlus(string func, int &i)
{
	int ob = 0;
	int cb = 0;
	do
	{
		if (func[i] == '(')
			ob++;
		if (func[i] == ')')
			cb++;
		i++;
	} while (ob != cb);//Доходим до первой операции
}

void skipBracketsMinus(string func, int &i)
{
	int ob = 0;
	int cb = 0;
	do
	{
		if (func[i] == '(')
			ob++;
		if (func[i] == ')')
			cb++;
		i--;
	} while (ob != cb);//Доходим до первой операции
}

void deleteExcessiveBrackets(string &func)
{
	string initialopz = exprToOpz(func);
	for (int i = 0; i < func.length(); i++)
	{
		string attempt = func;
		string attemptopz;
		if (attempt[i] == '(')
		{
			int start = i;
			skipBracketsPlus(attempt, i);
			i --;
			attempt.erase(i, 1);//Удалили скобочки у attempt
			attempt.erase(start, 1);
			attemptopz = exprToOpz(attempt);//Переводим в опз
			if (attemptopz == initialopz)//Скобочки можно удалить
			{
				func = attempt;
				i = start - 1;
			}
			else
				i = start;
		}
	}
}


void AssembleOperation(string &func, int realpos, int &posorig)
{
	//cout << "realpos = " << realpos << endl;
	//cout << "posorig = " << posorig << endl;
	//cout << func << endl;
	//cout << "Assembling , " << "func[" << realpos << "]=" << func[realpos] << endl;

	if (symboltype(func[realpos]) == 4)
	{
		if ((symboltype(func[realpos - 1]) == 4) || (symboltype(func[realpos - 1]) == 1))//Если правый операнд - операция
		{
			AssembleOperation(func, realpos-1, posorig);
		}
		else if ((symboltype(func[realpos - 1]) == 5) || (symboltype(func[realpos - 1]) == 6))//Если правый операнд - переменная(константа)
		{
			if (((symboltype(func[realpos - 2]) == 5) || (symboltype(func[realpos - 2]) == 6)))//Если левый операнд - переменная(константа)
			{
				char oper = func[realpos];//Операция
				func.erase(realpos, 1);//Убираем операцию с ее места
				func.insert(realpos - 2, "(");
				if (oper == '+')
					func.insert(realpos, "+");
				else if (oper == '*')
					func.insert(realpos, "*");
				else if (oper == '^')
					func.insert(realpos, "^");
				else if (oper == '@')
					func.insert(realpos, "@");
				else if (oper == '#')
					func.insert(realpos, "#");
				func.insert(realpos + 2, ")");
				posorig += 2;
				//cout << "realpos = " << realpos << endl;
				//cout << "posorig = " << posorig << endl;
				//cout << func << endl;
			}
			else if (func[realpos - 2] == ')')//Если левый операнд в скобочках
			{
				//cout << "func[" << realpos - 2 << "]=" << func[realpos - 2] << endl;
				char oper = func[realpos];//Операция
				func.erase(realpos, 1);//Убираем операцию с ее места
				func.insert(realpos, ")");
				if (oper == '+')
					func.insert(realpos - 1, "+");
				else if (oper == '*')
					func.insert(realpos - 1, "*");
				else if (oper == '^')
					func.insert(realpos - 1, "^");
				else if (oper == '@')
					func.insert(realpos - 1, "@");
				else if (oper == '#')
					func.insert(realpos - 1, "#");
				int smesh = realpos - 2;
				int ob = 0;
				int cb = 0;
				do
				{
					if (func[smesh] == '(')
						ob++;
					if (func[smesh] == ')')
						cb++;
					smesh--;
				} while (ob != cb);
				smesh++;
				func.insert(smesh, "(");
				posorig += 2;
				//cout << "func[" << realpos - 2 << "]=" << func[realpos - 2] << endl;
				//cout << "realpos = " << realpos << endl;
				//cout << "posorig = " << posorig << endl;
			}
			else if (symboltype(func[realpos - 2]) == 4)//Если левый операнд - бинарная операция
			{
				AssembleOperation(func, realpos-2, posorig);
			}
			else if (symboltype(func[realpos - 2]) == 1)//Если левый операнд - отрицание(инверсия)
			{
				AssembleOperation(func, realpos-2, posorig);
			}

		}
		else if (func[realpos - 1] == ')')//правый операнд в скобочках
		{
			int smesh = realpos - 1;
			int ob = 0;
			int cb = 0;
			do
			{
				if (func[smesh] == '(')
					ob++;
				if (func[smesh] == ')')
					cb++;
				smesh--;
			} while (ob != cb);
			if (func[smesh] == ')')//и левый операнд в скобочках
			{
				char oper = func[realpos];//Операция
				func.erase(realpos, 1);//Убираем операцию с ее места
				if (oper == '+')
					func.insert(smesh+1, "+");
				else if (oper == '*')
					func.insert(smesh+1, "*");
				else if (oper == '^')
					func.insert(smesh+1, "^");
				else if (oper == '@')
					func.insert(smesh+1, "@");
				else if (oper == '#')
					func.insert(smesh+1, "#");
				func.insert(realpos, ")");
				skipBracketsMinus(func, smesh);
				func.insert(smesh+1, "(");
				posorig += 2;
				//cout << "realpos = " << realpos << endl;
				//cout << "posorig = " << posorig << endl;
				//cout << func << endl;
			}
			else if ((symboltype(func[smesh]) == 5) || (symboltype(func[smesh]) == 6))
			{
				char oper = func[realpos];//Операция
				func.erase(realpos, 1);//Убираем операцию с ее места
				if (oper == '+')
					func.insert(smesh+1, "+");
				else if (oper == '*')
					func.insert(smesh+1, "*");
				else if (oper == '^')
					func.insert(smesh+1, "^");
				else if (oper == '@')
					func.insert(smesh+1, "@");
				else if (oper == '#')
					func.insert(smesh+1, "#");
				func.insert(realpos, ")");
				func.insert(smesh, "(");
				posorig += 2;
				//cout << "realpos = " << realpos << endl;
				//cout << "posorig = " << posorig << endl;
				//cout << func << endl;
			}
			else if (symboltype(func[smesh]) == 4)//Если левый операнд - бинарная операция
			{
				AssembleOperation(func, smesh, posorig);
			}
			else if (symboltype(func[smesh]) == 1)//Если левый операнд - отрицание(инверсия)
			{
				AssembleOperation(func, smesh, posorig);
			}
		}
		if (((symboltype(func[posorig]) == 4) || (symboltype(func[posorig]) == 1)))
			AssembleOperation(func, posorig, posorig);
	}
	else if (symboltype(func[realpos]) == 1)
	{
		if ((symboltype(func[realpos - 1]) == 5) || (symboltype(func[realpos - 1]) == 6))//Если слева операнд без скобок(константа или переменная)
		{
			func.erase(realpos, 1);//Убираем операцию с ее места
			func.insert(realpos, ")");
			func.insert(realpos - 1, "(~");
			posorig += 2;
			//cout << "realpos = " << realpos << endl;
			//cout << "posorig = " << posorig << endl;
			//cout << func << endl;
		}
		else if (func[realpos - 1] == ')')//Если слева операнд в скобках
		{
			func.erase(realpos, 1);
			func.insert(realpos, ")");
			int smesh = realpos - 1;
			skipBracketsMinus(func, smesh);
			smesh++;
			func.insert(smesh, "(~");
			posorig += 2;
			//cout << "realpos = " << realpos << endl;
			//cout << "posorig = " << posorig << endl;
			//cout << func << endl;
		}
		else if ((symboltype(func[realpos - 1]) == 4)|| (symboltype(func[realpos - 1]) == 1))
		{
			AssembleOperation(func, realpos-1,posorig);
			if (((symboltype(func[posorig])==4)||(symboltype(func[posorig])==1)))
				AssembleOperation(func, posorig,posorig);
		}
	}
}

int assembleLeftOperands(string &opzfuncchanged, int n)
{
	//cout << "opzfuncchanged = " << opzfuncchanged << endl;
	//cout << "n = " << n << endl;
	if (symboltype(opzfuncchanged[n]) == 4)//Если первая операция места применения формулы в функции бинарная
	{
		n--;
		AssembleOperation(opzfuncchanged, n, n);
		//cout << "opzfuncchanged = " << opzfuncchanged << endl;
		//cout << "opzfuncchanged [ " << n << " ] = " << opzfuncchanged[n] << endl;
		//Сейчас n - крайняя правая скобка раскрытой операции
		skipBracketsMinus(opzfuncchanged, n);
		//n - номер символа, стоящего слева от операнда
		//cout << "opzfuncchanged [ " << n << " ] = " << opzfuncchanged[n] << endl;
		if ((symboltype(opzfuncchanged[n]) == 4) || (symboltype(opzfuncchanged[n]) == 1))//Обрабатываем 2-ю операцию
		{
			AssembleOperation(opzfuncchanged, n, n);
			//cout << "opzfuncchanged = " << opzfuncchanged << endl;
			//Сейчас n - крайняя правая скобка раскрытой операции
			skipBracketsMinus(opzfuncchanged, n);
			n++;
			//n - индекс левой скобки первого операнда бинарной операции(начало места применения формулы)
		}//Если тут стоит переменная или константа, то ничего обрабатывать не нужно
		//cout << "opzfuncchanged [ "<<n<<" ] = " << opzfuncchanged[n] << endl;

	}
	else if (symboltype(opzfuncchanged[n]) == 1)//Если первая операция места применения формулы в функции унарная
	{
		n--;
		AssembleOperation(opzfuncchanged, n, n);
		//Сейчас n - крайняя правая скобка раскрытой операции
		skipBracketsMinus(opzfuncchanged, n);
		n++;
	}
	return n;
}

vector<int> findIndex(string opzformula1, string opzfunc, int &start_index_formula)
{
	//На вход opzformula и opzfunc
	//На выходе index[]
	//Инициализиция
	int n = -1;//Номер операции, найденной в функции
	vector<int> index;
	int skips = 0;//Количество переменных(констант) в текущей последовательности в формуле
	int actualskips = 0;//Количество переменных(констант) в текущей последовательности в функции
	int attempts = 0;//Количество возможных попыток найти применение формулы в функции
	int attemptn = 0;//Номер текущей попытки
	if (opzformula1.length() == 1)//В формуле переменная или константа
	{
		for (int n = 0; n < opzfunc.length(); n++)
		{
			if (symboltype(opzformula1[0]) == 5)//В формуле константа
			{
				if (opzfunc[n] == opzformula1[0])//Ищем в выражении только константы, совпадающие с той, которую нужно найти
				{
					//cout << "good attempt with n = " << n << endl;
					index.push_back(n);
				}
			}
			else if (symboltype(opzformula1[0]) == 6)//В формуле переменная
			{
				if ((symboltype(opzfunc[n]) == 5) || (symboltype(opzfunc[n]) == 6))//Ищем в формуле и переменные, и константы
				{
					//cout << "good attempt with n = " << n << endl;
					index.push_back(n);
				}
			}
		}
	}
	else
	{
		for (int i = 0; (i < opzformula1.length()) && (n == -1); i++)
		{
			if ((symboltype(opzformula1[i]) == 4) || (symboltype(opzformula1[i]) == 1))//Нашли операцию в формуле
			{
				start_index_formula = i;//Сохраняем номер данной операции в формуле
				for (int j = 0; j < opzfunc.length(); j++)
					if (opzfunc[j] == opzformula1[i])//Находим операцию,соответствующую операции в формуле
					{
						attempts++;
						n = j;
					}
				i = opzformula1.length();
			}
		}
		attemptn = 0;
		if (attempts != 0)
		{
			do
			{
				n = -1;
				for (int i = 0; (i < opzformula1.length()) && (n == -1); i++)
				{
					int k = 0;
					if ((symboltype(opzformula1[i]) == 4) || (symboltype(opzformula1[i]) == 1))//Нашли операцию в формуле
					{
						start_index_formula = i;//Сохраняем номер данной операции в формуле
						for (int j = 0; j < opzfunc.length(); j++)
							if (opzfunc[j] == opzformula1[i])//Находим операцию,соответствующую операции в формуле
							{
								if (k != attemptn)//Пока не пропустили уже рассмотренные позиции
								{
									k++;
								}
								else
								{
									attemptn++;//Попытка номер 1,2,...
									n = j;//Сохраняем номер данной операции в функции
									j = opzfunc.length();//Завершение цикла
								}
							}
						i = opzformula1.length();
					}
				}
				//cout << "index = " << start_index_formula << endl;
				//cout << "n = " << n << endl;
				//cout << "attemptn = " << attemptn << endl;
				//cout << opzfunc[n] << " ";
				//cout << opzformula[index] << endl;
				//Поиск
				int j = n;
			    //cout << "start_index_formula = " << start_index_formula << endl;
				//cout << "opzformula1 = " << opzformula1 << endl;
				if (start_index_formula + 1 == opzformula1.length())//Единственная операция
				{
					//cout << "good attempt with n = " << n << endl;
					index.push_back(n);
				}
				for (int i = start_index_formula + 1; i < opzformula1.length(); i++)
				{
					skips = 0;
					actualskips = 0;
					//cout << "1 opzformula[" << i << "]" << opzformula[i] << endl;
					while ((symboltype(opzformula1[i]) == 5) || (symboltype(opzformula1[i]) == 6))//Пока нашли переменную или константу
					{
						skips++;
						i++;
						//cout << "formula " << opzformula[i] << endl;
					}
					//cout << "skips = " << skips << endl;
					j++;
					if (j == opzfunc.length())//Если вышли за пределы функции
					{
						//cout << "bad attempt 0" << endl;
						i = opzformula1.length();
					}
					else
					{
						//cout << "1 opzfunc[" << j << "]" << opzfunc[j] << endl;
						while ((symboltype(opzfunc[j]) == 5) || (symboltype(opzfunc[j]) == 6))//Пока нашли переменную или константу
						{
							actualskips++;
							j++;
							//cout << "func " << opzfunc[j] << endl;
						}
						//cout << "actualskips = " << actualskips << endl;
						//cout << "2 opzformula[" << i << "]" << opzformula[i] << endl;
						//cout << "2 opzfunc[" << j << "]" << opzfunc[j] << endl;

						if (skips > actualskips)
						{
							i = opzformula1.length();
							//cout << "bad attempt 1" << endl;
						}
						else
						{
							int difference = actualskips - skips;
							while (difference > 0)
							{
								if (symboltype(opzfunc[j]) == 4)//Если бинарная операция
									difference--;
								else if ((symboltype(opzfunc[j]) == 5) || (symboltype(opzfunc[j]) == 6))
									difference++;
								j++;
								if (j == opzfunc.size())
								{
									//cout << "bad attempt 2" << endl;
									i = opzformula1.length();
								}
							}
							//cout << "opzformula[" << i << "]" << opzformula[i] << endl;
							//cout << "opzfunc[" << j << "]" << opzfunc[j] << endl;
							int tempj;
							int tempi;
							//cout << "skips = " << skips << endl;
							//cout << "actualskips = " << actualskips << endl;
							if ((skips || actualskips) && (symboltype(opzfunc[j]) == 1))//Если отрицание
							{
								//cout << "here" << endl;
								while (symboltype(opzfunc[j + 1]) == 1)//Доходим до последнего отрицания в этом месте
									j++;
								//cout << "           opzformula[" << i << "]" << opzformula[i] << endl;
								if ((opzfunc[j] != opzformula1[i]) && (opzfunc[j + 1] == opzformula1[i]))//Если в формуле следующая после отрицания операция
									j++;
								else if (opzfunc[j] != opzformula1[i])//Если в формуле не отрицание
								{
									//cout << "Bad attempt 3" << endl;
									i = opzformula1.length();
								}
								else if (opzfunc[j] == opzformula1[i])
								{//Если в формуле отрицание
									tempj = j;
									tempi = i;
									while (opzfunc[tempj] == opzformula1[tempi])
									{
										tempj--;
										tempi++;
									}
									if ((symboltype(opzformula1[i - 1]) == 5) || (symboltype(opzformula1[i - 1]) == 6))//Если в формуле предыдущей была переменная(константа)
									{
										//cout << "here1" << endl;
										if (symboltype(opzformula1[tempi]) == 1)
										{
											//cout << "Bad attempt 4" << endl;
											i = opzformula1.length();
										}
										else
										{
											//while (symboltype(opzfunc[j+1]) == 1)
											//	j++;
											while (symboltype(opzformula1[i + 1]) == 1)
												i++;
										}
									}
									else
									{
										//cout << "here2" << endl;
										if ((symboltype(opzformula1[tempi]) == 1) || (symboltype(opzfunc[tempj]) == 1))
										{
											//cout << "Badd attempt 4" << endl;
											i = opzformula1.length();
										}
										else
										{
											//while (symboltype(opzfunc[j+1]) == 1)
											//	j++;
											while (symboltype(opzformula1[i + 1]) == 1)
												i++;
										}
									}

									//j++;
									//i++;
									//cout << " i = " << i << " j = " << j << endl;
								}

							}
							//cout << "opzformula[" << i << "]" << opzformula[i] << endl;
							//cout << "opzfunc[" << j << "]" << opzfunc[j] << endl;
							if ((opzformula1[i] != opzfunc[j]))
							{
								//cout << "Bad attempt 5" << endl;
								i = opzformula1.length();
							}
							else if (i == opzformula1.length() - 1)//Дошли до конца формулы
							{
								index.push_back(n);
								//cout << "Trying for identical variables and constants" << endl;
								//cout << "n = " << n << endl;
							}
						}
					}
				}
				attempts--;
				// cout << "attempts = " << attempts << endl;
			} while (attempts > 0);
		}
	}
	return index;
}


int ifIndexCorrect(string opzfuncchanged, string opzformula1, int n)
{
	//Нахождение массивов переменных формулы
	//cout << "                   opzfuncchanged = " << opzfuncchanged << endl;
	//cout << "   start - opzfuncchanged[" << n << "] = " << opzfuncchanged[n] << endl;
	vector<string> varsformula1;
	vector<string> varsfunc;
	//k - индекс прохождения по формуле, h - индекс прохождения по функции
	for (int k = 0, h = n; k < opzformula1.length(); k++, h++)
	{
		if ((symboltype(opzformula1[k]) != 4) && (symboltype(opzformula1[k]) != 1))//Если текущий символ в формуле(соответственно, в функции) не операция
		{
			int m = 0;//длина текущего операнда в функции
			int ob = 0;
			int cb = 0;
			do
			{
				if (opzfuncchanged[h] == '(')
					ob++;
				if (opzfuncchanged[h] == ')')
					cb++;
				h++;
				m++;
			} while (ob != cb);//Доходим до первой операции

			string temp = opzfuncchanged.substr(h - m, m);
			//cout << "temp = " << temp << endl;
			h--;
			varsfunc.push_back(temp);
			temp = opzformula1.substr(k, 1);
			varsformula1.push_back(temp);
		}
	}
	int flag = 1;
	for (int b = 0; b < varsfunc.size(); b++)
		for (int h = 0; h < varsfunc.size(); h++)
		{
			//cout << varsfunc[b] << "!=" << varsfunc[h] << endl;
			//cout << varsformula1[b] << "==" << varsformula1[h] << endl;
			//cout << endl;
			if (varsformula1[b] == "0")
			{
				if (varsfunc[b] != "0")
				{
					h = varsfunc.size();
					b = varsfunc.size();
					flag = 0;
				}
			}
			else if (varsformula1[b] == "1")
			{
				if (varsfunc[b] != "1")
				{
					h = varsfunc.size();
					b = varsfunc.size();
					flag = 0;
				}
			}
			else if ((varsfunc[b] != varsfunc[h]) && (varsformula1[b] == varsformula1[h]))
			{
				h = varsfunc.size();
				b = varsfunc.size();
				flag = 0;
			}
		}
	/*for (int b = 0; b < varsfunc.size(); b++)
	{
		cout << varsformula1[b] << " - " << varsfunc[b] << endl;
	}*/
	return flag;
}

//vector<int> findOpzFuncChanged(string opzfunc, string opzformula1, int start_index_formula, int start_index_func)
//{

//}

vector<int> findCorrectIndex(string opzfunc, string opzformula1, int start_index_formula, vector<int> index)
{
	vector<int> indexchanged;
	string opzfuncchanged;
	//На входе opzfunc и index[]
	for (int u = 0; u < index.size(); u++)
	{
		int n = index[u];
		//cout << "   index[" << u << "] = " << n << endl;
		opzfuncchanged = opzfunc;
		//cout << "   opzfuncchanged = " << opzfuncchanged << endl;
		//cout << "   correctindex = " << n << endl;
		//cout << "   i = " << start_index_formula << endl;
		int i = start_index_formula;
		//На входе opzfunc и index[]
		//"Соберем" операции функции, которых нет в формуле
		int j = n;
		//cout << "   opzfuncchanged = " << opzfuncchanged << endl;
		for (; (i < opzformula1.length()) && (j < opzfuncchanged.length()); j++)
		{
			int operandstoassemble;
			if (symboltype(opzfuncchanged[j]) == 1)
				operandstoassemble = 1;
			else if (symboltype(opzfuncchanged[j]) == 4)
				operandstoassemble = 2;
			else
				operandstoassemble = 0;
			//Здесь мы находимся на первой "Правильной" операции(операнде) в функции(и в формуле)
			//Найдем правый и левый операнд операции, если там операция, стоящая левее индекса начала применения, то превратим в операнд
			//cout << "   j = " << j << endl;
			for (int l = j-1; operandstoassemble; l--)
			{
				//cout << "   opzfuncchanged = " << opzfuncchanged << endl;
				//cout << "   operandstoassemble = " << operandstoassemble << endl;
				if (symboltype(opzfuncchanged[l]) == 4)
				{
					if (l < n)
					{
						int temp = l;
						AssembleOperation(opzfuncchanged, l, l);
						j += l - temp;
						skipBracketsMinus(opzfuncchanged, l);
						l++;
						if (l < n)
							n = l;
						operandstoassemble--;
					}
					else
						operandstoassemble++;
				}
				else if (symboltype(opzfuncchanged[l]) == 1)
				{
					if (l < n)
					{
						int temp = l;
						AssembleOperation(opzfuncchanged, l, l);
						j += l - temp;
						skipBracketsMinus(opzfuncchanged, l);
						l++;
						if (l < n)
							n = l;
						operandstoassemble--;
					}
				}
				else if ((symboltype(opzfuncchanged[l]) == 5) || (symboltype(opzfuncchanged[l]) == 6))//Если переменная или константа
				{
					operandstoassemble--;
					if (l < n)
						n = l;
				}
				else if (opzfuncchanged[l] == ')')
				{
					skipBracketsMinus(opzfuncchanged, l);
					l++;
					if (l < n)
						n = l;
					operandstoassemble--;
				}
			}
			//cout << "   opzfuncchanged = " << opzfuncchanged << endl;
			//cout << "   operandstoassemble = " << operandstoassemble << endl;
			int skips = 0;
			int actualskips = 0;
			while ((symboltype(opzformula1[i]) == 5) || (symboltype(opzformula1[i]) == 6))
			{
				i++;
				skips++;
			}
			actualskips = 0;
			while ((symboltype(opzfuncchanged[j]) == 5) || (symboltype(opzfuncchanged[j]) == 6))//Пока нашли переменную или константу
			{
				actualskips++;
				j++;
			}
			int difference = actualskips - skips;
			while (difference > 0)
			{
				if (symboltype(opzfuncchanged[j]) == 4)//Если бинарная операция
				{
					difference--;
					if (j != opzfuncchanged.length() - 1)
					{
						AssembleOperation(opzfuncchanged, j, j);
					}
				}
				else if ((symboltype(opzfuncchanged[j]) == 5) || (symboltype(opzfuncchanged[j]) == 6))
				{
					difference++;
				}
				j++;
			}
			if (actualskips)
			{
				if (symboltype(opzfuncchanged[j]) == 1)//Если отрицание
				{
					while (symboltype(opzfuncchanged[j + 1]) == 1)//Доходим до последнего отрицания в этом месте
					{
						j++;

					}
					if ((opzfuncchanged[j] != opzformula1[i]) && (opzfuncchanged[j + 1] == opzformula1[i]))//Если в формуле следующая после отрицания операция
					{
						AssembleOperation(opzfuncchanged, j, j);
						j++;
					}
					//Иначе остановились на последнем отрицании
					else if (opzfuncchanged[j] == opzformula1[i])
					{//Если в формуле отрицание
						int tempj = j;
						int tempi = i;
						while (opzfuncchanged[tempj] == opzformula1[tempi])
						{
							tempj--;
							tempi++;
						}//j - tempj - количество минусов в формуле
						int amt = 0;
						while (j - tempj > 0)
						{
							j--;
							amt++;
						}
						while (amt)
						{
							AssembleOperation(opzfuncchanged, j, j);
							j++;
							amt--;
						}
						while (symboltype(opzformula1[i + 1]) == 1)
							i++;
					}

				}

			}
			i++;
		}
		if (ifIndexCorrect(opzfuncchanged, opzformula1, n))
			indexchanged.push_back(index[u]);
	}
	return indexchanged;
}

vector<string> findVarsFunc(int correctindex, string opzfunc, string opzformula1, int i)
{
	string opzfuncchanged = opzfunc;
	//На входе opzfunc и index[]
	//"Соберем" операции функции, которых нет в формуле
	int j = correctindex;
	for (; (i < opzformula1.length()) && (j < opzfuncchanged.length()); j++)
	{
		int operandstoassemble;
		if (symboltype(opzfuncchanged[j]) == 1)
			operandstoassemble = 1;
		else if (symboltype(opzfuncchanged[j]) == 4)
			operandstoassemble = 2;
		else
			operandstoassemble = 0;
		//Здесь мы находимся на первой "Правильной" операции(операнде) в функции(и в формуле)
		//Найдем правый и левый операнд операции, если там операция, стоящая левее индекса начала применения, то превратим в операнд
		//cout << "   j = " << j << endl;
		for (int l = j - 1; operandstoassemble; l--)
		{
			//cout << "   opzfuncchanged = " << opzfuncchanged << endl;
			//cout << "   operandstoassemble = " << operandstoassemble << endl;
			if (symboltype(opzfuncchanged[l]) == 4)
			{
				if (l < correctindex)
				{
					int temp = l;
					AssembleOperation(opzfuncchanged, l, l);
					j += l - temp;
					skipBracketsMinus(opzfuncchanged, l);
					l++;
					if (l < correctindex)
						correctindex = l;
					operandstoassemble--;
				}
				else
					operandstoassemble++;
			}
			else if (symboltype(opzfuncchanged[l]) == 1)
			{
				if (l < correctindex)
				{
					int temp = l;
					AssembleOperation(opzfuncchanged, l, l);
					j += l - temp;
					skipBracketsMinus(opzfuncchanged, l);
					l++;
					if (l < correctindex)
						correctindex = l;
					operandstoassemble--;
				}
			}
			else if ((symboltype(opzfuncchanged[l]) == 5) || (symboltype(opzfuncchanged[l]) == 6))//Если переменная или константа
			{
				operandstoassemble--;
				if (l < correctindex)
					correctindex = l;
			}
			else if (opzfuncchanged[l] == ')')
			{
				skipBracketsMinus(opzfuncchanged, l);
				l++;
				if (l < correctindex)
					correctindex = l;
				operandstoassemble--;
			}
		}
		int skips = 0;
		int actualskips = 0;
		while ((symboltype(opzformula1[i]) == 5) || (symboltype(opzformula1[i]) == 6))
		{
			i++;
			skips++;
		}
		actualskips = 0;
		while ((symboltype(opzfuncchanged[j]) == 5) || (symboltype(opzfuncchanged[j]) == 6))//Пока нашли переменную или константу
		{
			actualskips++;
			j++;
		}
		int difference = actualskips - skips;
		while (difference > 0)
		{
			if (symboltype(opzfuncchanged[j]) == 4)//Если бинарная операция
			{
				difference--;
				if (j != opzfuncchanged.length() - 1)
				{
					AssembleOperation(opzfuncchanged, j, j);
				}
			}
			else if ((symboltype(opzfuncchanged[j]) == 5) || (symboltype(opzfuncchanged[j]) == 6))
			{
				difference++;
			}
			j++;
		}
		if (actualskips)
		{
			if (symboltype(opzfuncchanged[j]) == 1)//Если отрицание
			{
				while (symboltype(opzfuncchanged[j + 1]) == 1)//Доходим до последнего отрицания в этом месте
				{
					j++;

				}
				if ((opzfuncchanged[j] != opzformula1[i]) && (opzfuncchanged[j + 1] == opzformula1[i]))//Если в формуле следующая после отрицания операция
				{
					AssembleOperation(opzfuncchanged, j, j);
					j++;
				}
				//Иначе остановились на последнем отрицании
				else if (opzfuncchanged[j] == opzformula1[i])
				{//Если в формуле отрицание
					int tempj = j;
					int tempi = i;
					while (opzfuncchanged[tempj] == opzformula1[tempi])
					{
						tempj--;
						tempi++;
					}//j - tempj - количество минусов в формуле
					int amt = 0;
					while (j - tempj > 0)
					{
						j--;
						amt++;
					}
					while (amt)
					{
						AssembleOperation(opzfuncchanged, j, j);
						j++;
						amt--;
					}
					while (symboltype(opzformula1[i + 1]) == 1)
						i++;
				}

			}

		}
		i++;
	}
	//Нахождение массивов переменных формулы
	//cout << "..............opzfuncchanged = " << opzfuncchanged << endl;
	//cout << "correct index = " << correctindex << endl;
	//cout << "start - opzfuncchanged[" << n << "] = " << opzfuncchanged[n] << endl;
	vector<string> varsfunc;
	//k - индекс прохождения по формуле, h - индекс прохождения по функции
	for (int k = 0, h = correctindex; k < opzformula1.length(); k++, h++)
	{
		if ((symboltype(opzformula1[k]) != 4) && (symboltype(opzformula1[k]) != 1))//Если текущий символ в формуле(соответственно, в функции) не операция
		{
			int m = 0;//длина текущего операнда в функции
			int ob = 0;
			int cb = 0;
			do
			{
				if (opzfuncchanged[h] == '(')
					ob++;
				if (opzfuncchanged[h] == ')')
					cb++;
				h++;
				m++;
			} while (ob != cb);//Доходим до первой операции

			string temp = opzfuncchanged.substr(h - m, m);
			h--;
			varsfunc.push_back(temp);
		}
	}
	/*for (int b = 0; b < varsfunc.size(); b++)
	{
		cout << varsfunc[b] << endl;
	}*/
	return varsfunc;
}

vector<string> findVarsFormula(string opzformulai)
{
	vector<string> varsfunc;
	for (int k = 0; k < opzformulai.length(); k++)
	{
		if ((symboltype(opzformulai[k]) != 4) && (symboltype(opzformulai[k]) != 1))//Если текущий символ в формуле(соответственно, в функции) не операция
		{
			varsfunc.push_back(opzformulai.substr(k, 1));
		}
	}
	return varsfunc;
}

vector<char> findVarsFormulaWithoutDuplicates(string opzformulai)
{
	vector<char> varsfunc;
	for (int k = 0; k < opzformulai.length(); k++)
	{
		if (symboltype(opzformulai[k]) == 6)//Если текущий символ переменная
		{
			int flag = 1;
			for (int j = 0; j < k; j++)
				if (opzformulai[k] == opzformulai[j])
					flag = 0;
			if (flag)
			{
				varsfunc.push_back(opzformulai[k]);
			}
		}
	}
	if (varsfunc.size()!=0)
		for (int i = 0; i < varsfunc.size() - 1; i++)
		{
			for (int j = i + 1; j < varsfunc.size(); j++)
				if (varsfunc[i] > varsfunc[j])
				{
					int temp = varsfunc[i];
					varsfunc[i] = varsfunc[j];
					varsfunc[j] = temp;
				}
		}
	return varsfunc;
}

int truthTableLine(string opzformulai, vector<int> vals, vector<char> vars)
{
	stack<int> st;
	int n1, n2, res;
	//Дано какое-то количество переменных
	for (int i = 0; i < opzformulai.length(); i++)
	{
		if (symboltype(opzformulai[i]) == 5)//Если константа
		{
			if (opzformulai[i] == '1')
				st.push(1);
			else if (opzformulai[i] == '0')
				st.push(0);
		}
		else if (symboltype(opzformulai[i]) == 6)//Если переменная
		{
			for (int k = 0; k < vals.size(); k++)
				if (opzformulai[i] == vars[k])
				{
					st.push(vals[k]);
					k = vals.size();
				}
		}
		else if (symboltype(opzformulai[i]) == 4)//Если бинарная операция
		{
			n2 = st.top();
			st.pop();
			n1 = st.top();
			st.pop();
			switch (opzformulai[i])
			{
			case '+': res = n1|n2; break;
			case '*': res = n1&n2; break;
			case '^': res = ((!n1)&n2)|(n1&(!n2)); break;
			case '@': res = (!n1) | (n2); break;
			case '#': res = ((!n1) & (!n2))|(n1&n2); break;
			default: cout << "Ошибка !" << endl;
			}
			st.push(res);
		}
		else if (symboltype(opzformulai[i]) == 1)//Если отрицание
		{
			n1 = st.top();
			st.pop();
			res = !n1;
			st.push(res);
		}
	}
	n1 = st.top();
	st.pop();
	return n1;
}

int truthTable(string opzformula1, string opzformula2)
{
	vector<char> vars1;
	vector<int> result1;
	int amtvars1;
	//vars = findVarsFormula(opzformulai);
	vars1 = findVarsFormulaWithoutDuplicates(opzformula1);
	amtvars1 = vars1.size();
	vector<vector<int>> table1;
	int M = 1 << amtvars1;
	table1.resize(M);
	for (int i = 0; i < table1.size(); i++)
		table1[i].resize(amtvars1);
	int j;
	for (j = 0; j < amtvars1; j++) table1[0][j] = 0;
	for (int i = 1; i < M; i++)
	{
		for (j = amtvars1 - 1; j >= 0; j--)
		{
			if (table1[i - 1][j] == 0)
			{
				table1[i][j] = 1;
				break;
			}
			else
				table1[i][j] = 0;
		}
		for (j--; j >= 0; j--)
			table1[i][j] = table1[i - 1][j];
	}
	/*cout << "Truth table1: " << endl;
	for (int i = 0; i < table1.size(); i++)
	{
		for (int j = 0; j < table1[i].size(); j++)
			cout << table1[i][j];
		cout << endl;
	}*/
	for (int i = 0; i < table1.size(); i++)
	{
		result1.push_back(truthTableLine(opzformula1, table1[i], vars1));
	}
	//for (int i = 0; i < result1.size(); i++)
	//	cout << "result1 = " << result1[i] << endl;

	vector<char> vars2;
	vector<int> result2;
	int amtvars2;
	//vars = findVarsFormula(opzformulai);
	vars2 = findVarsFormulaWithoutDuplicates(opzformula2);
	amtvars2 = vars2.size();
	vector<vector<int>> table2;
	M = 1 << amtvars2;
	table2.resize(M);
	for (int i = 0; i < table2.size(); i++)
		table2[i].resize(amtvars2);
	for (j = 0; j < amtvars2; j++) table2[0][j] = 0;
	for (int i = 1; i < M; i++)
	{
		for (j = amtvars2 - 1; j >= 0; j--)
		{
			if (table2[i - 1][j] == 0)
			{
				table2[i][j] = 1;
				break;
			}
			else
				table2[i][j] = 0;
		}
		for (j--; j >= 0; j--)
			table2[i][j] = table2[i - 1][j];
	}
	/*cout << "Truth table2: " << endl;
	for (int i = 0; i < table2.size(); i++)
	{
		for (int j = 0; j < table2[i].size(); j++)
			cout << table2[i][j];
		cout << endl;
	}*/
	for (int i = 0; i < table2.size(); i++)
	{
		result2.push_back(truthTableLine(opzformula2, table2[i], vars2));
	}
	//for (int i = 0; i < result2.size(); i++)
	//	cout << "result2 = " << result2[i] << endl;
	//Найдем разницу между размерами результатов
	int difference = 1;
	int temp1 = result1.size();
	int temp2 = result2.size();
	if (result1.size() < result2.size())
	{
		while (temp1 < temp2)
		{
			temp1 = temp1 << 1;
			difference = difference << 1;
		}
		for (int n = 0; n < result2.size(); n++)
			if (result2[n] != result1[n/difference])
				return 0;
		return 1;
	}
	else if (result1.size() > result2.size())
	{
		while (temp1 > temp2)
		{
			temp2 = temp2 << 1;
			difference = difference << 1;
		}
		for (int n = 0; n < result1.size(); n++)
			if (result1[n] != result2[n/difference])
				return 0;
		return 1;
	}
	else
		return result1 == result2;
}


void splitFormula(string formula, string &formula1, string &formula2)
{
	for (int i = 0; i < formula.length(); i++)
	{
		if (formula[i] == '=')
		{
			formula1 = formula.substr(0, i);
			formula2 = formula.substr(i+1, formula.length() - i - 1);
		}
	}
}

void eraseFormulaInFunc(string opzfunc, string &opzfuncoutput, string opzformula1, string opzfuncinsert, int indexoperation, int start_index_formula)
{
	int j = indexoperation;
	int i = start_index_formula;
	for (; (i < opzformula1.length()) && (j < opzfuncoutput.length()); j++)
	{
		int operandstoassemble;
		if (symboltype(opzfuncoutput[j]) == 1)
			operandstoassemble = 1;
		else if (symboltype(opzfuncoutput[j]) == 4)
			operandstoassemble = 2;
		else
			operandstoassemble = 0;
		//Здесь мы находимся на первой "Правильной" операции(операнде) в функции(и в формуле)
		//Найдем правый и левый операнд операции, если там операция, стоящая левее индекса начала применения, то превратим в операнд
		//cout << "   j = " << j << endl;
		for (int l = j - 1; operandstoassemble; l--)
		{
			//cout << "   opzfuncoutput = " << opzfuncoutput << endl;
			//cout << "   operandstoassemble = " << operandstoassemble << endl;
			if (symboltype(opzfuncoutput[l]) == 4)
			{
				if (l < indexoperation)
				{
					int temp = l;
					AssembleOperation(opzfuncoutput, l, l);
					j += l - temp;
					skipBracketsMinus(opzfuncoutput, l);
					l++;
					if (l < indexoperation)
						indexoperation = l;
					operandstoassemble--;
				}
				else
					operandstoassemble++;
			}
			else if (symboltype(opzfuncoutput[l]) == 1)
			{
				if (l < indexoperation)
				{
					int temp = l;
					AssembleOperation(opzfuncoutput, l, l);
					j += l - temp;
					skipBracketsMinus(opzfuncoutput, l);
					l++;
					if (l < indexoperation)
						indexoperation = l;
					operandstoassemble--;
				}
			}
			else if ((symboltype(opzfuncoutput[l]) == 5) || (symboltype(opzfuncoutput[l]) == 6))//Если переменная или константа
			{
				operandstoassemble--;
				if (l < indexoperation)
					indexoperation = l;
			}
			else if (opzfuncoutput[l] == ')')
			{
				skipBracketsMinus(opzfuncoutput, l);
				l++;
				if (l < indexoperation)
					indexoperation = l;
				operandstoassemble--;
			}
		}
		//cout << "         j = " << j << endl;

		//cout << "         opzfuncoutput = " << opzfuncoutput << endl;
		//cout << "   operandstoassemble = " << operandstoassemble << endl;
		int skips = 0;
		int actualskips = 0;
		while ((symboltype(opzformula1[i]) == 5) || (symboltype(opzformula1[i]) == 6))
		{
			i++;
			skips++;
		}
		actualskips = 0;
		while ((symboltype(opzfuncoutput[j]) == 5) || (symboltype(opzfuncoutput[j]) == 6))//Пока нашли переменную или константу
		{
			actualskips++;
			j++;
		}
		int difference = actualskips - skips;
		while (difference > 0)
		{
			if (symboltype(opzfuncoutput[j]) == 4)//Если бинарная операция
			{
				difference--;
				if (j != opzfuncoutput.length() - 1)
				{
					AssembleOperation(opzfuncoutput, j, j);
				}
			}
			else if ((symboltype(opzfuncoutput[j]) == 5) || (symboltype(opzfuncoutput[j]) == 6))
			{
				difference++;
			}
			j++;
		}
		//cout << "     ysdgsd    j = " << j << endl;
		//cout << "     ysdsg     i = " << i << endl;
		if (actualskips)
		{
			//cout << "here " << endl;
			if (symboltype(opzfuncoutput[j]) == 1)//Если отрицание
			{
				while (symboltype(opzfuncoutput[j + 1]) == 1)//Доходим до последнего отрицания в этом месте
				{
					j++;

				}
				if ((opzfuncoutput[j] != opzformula1[i]) && (opzfuncoutput[j + 1] == opzformula1[i]))//Если в формуле следующая после отрицания операция
				{
					AssembleOperation(opzfuncoutput, j, j);
					j++;
				}
				//Иначе остановились на последнем отрицании
				else if (opzfuncoutput[j] == opzformula1[i])
				{//Если в формуле отрицание
					int tempj = j;
					int tempi = i;
					while (opzfuncoutput[tempj] == opzformula1[tempi])
					{
						tempj--;
						tempi++;
					}//j - tempj - количество минусов в формуле
					int amt = 0;
					while (j - tempj > 0)
					{
						j--;
						amt++;
					}
					while (amt)
					{
						AssembleOperation(opzfuncoutput, j, j);
						j++;
						amt--;
					}
					while (symboltype(opzformula1[i + 1]) == 1)
						i++;
				}

			}

		}
		//cout << "....opzfuncoutput [ " << j << " ] = " << opzfuncoutput[j] << endl;
		//cout << "....opzformula1 [ " << i << " ] = " << opzformula1[i] << endl;
		i++;
	}
	int end = j - 1;
	//cout << "....opzfuncoutput [ " << end << " ] = " << opzfuncoutput[end] << endl;
	//cout << "....opzformula1 [ " << i - 1 << " ] = " << opzformula1[i - 1] << endl;
	int temp = indexoperation;//Открывающая скобка раскрытого(раскрытых) операндов
	//cout << "temp = " << temp << endl;
	//cout << "    opzfuncoutput = " << opzfuncoutput << endl;
	opzfuncoutput.erase(temp, end-temp+1);
	//cout << "    after erase   opzfuncoutput = " << opzfuncoutput << endl;
	opzfuncoutput.insert(temp, opzfuncinsert);
	//cout << "       opzfuncoutput = " << opzfuncoutput << endl;
	/*do
	{
		skipBracketsPlus(opzfuncoutput, indexoperation);
	} while ((symboltype(opzfuncoutput[indexoperation]) != 1) && (symboltype(opzfuncoutput[indexoperation]) != 4));
	//cout << "       indexoperation = " << indexoperation << endl;
	opzfuncoutput.erase(temp, findEndIndex(opzfuncoutput, opzformula1, start_index_formula, indexoperation) - temp +1);
	//cout << "    after erase   opzfuncoutput = " << opzfuncoutput << endl;
	opzfuncoutput.insert(temp, opzfuncinsert);
	//cout << "       opzfuncoutput = " << opzfuncoutput << endl;*/
}

string findOpzFuncInsert(string opzformula2, vector<string> varsformula1, vector<string> varsfunc)
{
	string opzfuncinsert;
	for (int j = 0; j < opzformula2.length(); j++)
	{
		if ((symboltype(opzformula2[j]) == 5) || (symboltype(opzformula2[j]) == 6))
		{
			int ind = -1;
			for (int k = 0; k < varsformula1.size(); k++)
			{
				string c = opzformula2.substr(j, 1);
				//cout << "varsfunc [ " << k << " ] = " << varsformula1[k] << endl;
				//cout << "c = " << c << endl;
				if (c == varsformula1[k])
				{
					ind = k;
					k = varsformula1.size();
				}
			}
			if (ind == -1)
				opzfuncinsert += opzformula2[j];
			else
				opzfuncinsert += varsfunc[ind];
			//cout << "opzformula2 [ " << j << " ] = " << opzformula2[j] << endl;
			//cout << "opzfuncinsert = " << opzfuncinsert << endl;
			//opzfuncinsert.insert(opzfuncinsert.length(), varsfunc[i]);
		}
		else if ((symboltype(opzformula2[j]) == 1) || (symboltype(opzformula2[j]) == 4))
		{
			switch (opzformula2[j])
			{
			case '~':
				opzfuncinsert.insert(opzfuncinsert.length(), "~");
				break;
			case '+':
				opzfuncinsert.insert(opzfuncinsert.length(), "+");
				break;
			case '*':
				opzfuncinsert.insert(opzfuncinsert.length(), "*");
				break;
			case '^':
				opzfuncinsert.insert(opzfuncinsert.length(), "^");
				break;
			case '@':
				opzfuncinsert.insert(opzfuncinsert.length(), "@");
				break;
			case '#':
				opzfuncinsert.insert(opzfuncinsert.length(), "#");
				break;
			}
		}
	}
	return opzfuncinsert;
}

int findNLineInFile(string &line, string filename, int ind)
{
	int flag = 1;
	ifstream fin(filename);
	if (!fin.is_open()) { cout << "Can't open input file named " << filename << " !" << endl; return 0; }
	for (int i = 1; (!fin.eof()) && flag; i++)
	{
		getline(fin, line);
		if (ind == i)
		{
			flag = 0;
		}
	}
	fin.close();
	return 1;
}

int outContentsOfFile(string filename)
{
	ifstream fin(filename);
	string line;
	if (!fin.is_open()) { cout << "Can't open input file named " << filename << " !" << endl; return 0; }
	for (int i = 1; !fin.eof(); i++)
	{
		getline(fin, line);
		cout << i << ") " << line << endl;
	}
	fin.close(); 
	return 1;
}

void clearFile(string filename)
{
	ofstream out(filename);
	if (!out.is_open()) { cerr << "Can't open input file named " << filename << " !" << endl; exit(1); }
	out.clear();
	out.close();
}

void printLinesInEndOfFile(vector<string> lines, string filename)
{
	ofstream out;
	out.open(filename, std::ios::app);
	if (!out.is_open()) { cerr << "Can't open input file named " << filename << " !" << endl; exit(1); }
	for (int i = 0; i < lines.size();i++)
		out << lines[i] << endl;
	out.close();
}

void printLineInEndOfFileWithClear(string line, string filename)
{
	ofstream out(filename);
	out.clear();
	if (!out.is_open()) { cerr << "Can't open input file named " << filename << " !" << endl; exit(1); }
	out << line << endl;
	out.close();
}

void printLineInEndOfFile(string line, string filename)
{
	ofstream out;
	out.open(filename, std::ios::app);
	if (!out.is_open()) { cerr << "Can't open input file named " << filename << " !" << endl; exit(1); }
	out << line << endl;
	out.close();
}

int main()
{
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//944 - голубой-черный, 244 - бело - красный, 240 - бело-черный, 15 - стандарт
	SetConsoleCP(1251); //установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); //установка кодовой страницы win-cp 1251 в поток 
	//SetConsoleTextAttribute(hConsole, 1);
	//system("cls");
	string formula;
	string formula1;
	string formula2;
	string func;
	string opzfunc;
	string funcoutput;
	string opzfuncoutput;
	string opzformula1;
	string opzformula2;
	vector<string> varsformula1;
	vector<string> varsfunc;
	vector<string> result;
	int dec, dec2;
	int n = 1;
	int flag;
	do
	{
		system("cls");
		cout << "---------------Цифровой помощник упрощения логических выражений---------------" << endl << endl;
		cout << "1.)Упрощение выражения." << endl;
		cout << "2.)Ввод выражения для упрощения." << endl;
		cout << "3.)Выбор выражения для упрощения из файла tasks.txt" << endl;
		cout << "4.)Ввод формул." << endl;
		cout << "5.)Справка." << endl;
		cout << "6.)Вывод на экран истории упрощений." << endl;
		cout << "7.)Очистить историю упрощений." << endl;
		cout << "0.)Выход." << endl;
		cout << "Ваш выбор: " << endl;
		cin >> dec;
		switch (dec)
		{
		case 1:
			do
			{
				system("cls");
				//EnterFromFile(func, formula);
				//cout << "Enter function: " << endl;
				//cin >> func;
				//cout << "Enter formula: " << endl;
				//cin >> formula;
				if (findNLineInFile(func, "expression.txt", 1) && findNLineInFile(formula, "formula.txt", n))
				{
					if ((correctnessOfFormula(formula)) && (correctnessOfExpression(func)) && (checkFormulaVariables(formula, func)))
					{
						cout << "Выражение = " << func << endl;
						cout << "Формула = " << formula << endl;
						splitFormula(formula, formula1, formula2);
						opzfunc = exprToOpz(func);
						opzformula1 = exprToOpz(formula1);
						opzformula2 = exprToOpz(formula2);
						//cout << "opzfunction: " << endl;
						//cout << opzfunc << endl;
						//cout << "opzformula1: " << endl;
						//cout << opzformula1 << endl;
						//cout << "opzformula2: " << endl;
						//cout << opzformula2 << endl;
						if (truthTable(opzformula1, opzformula2))
						{
							cout << "Формула верна." << endl;
							int start_index_formula = 0;
							vector<int> index;
							vector<int> indexchanged;
							index = findIndex(opzformula1, opzfunc, start_index_formula);
							if (index.size() != 0)
							{
								if ((symboltype(opzfunc[index[0]]) == 5) || (symboltype(opzfunc[index[0]]) == 6))//Если в формуле одна константа или переменная
								{
									indexchanged = index;
									for (int i = 0; i < indexchanged.size(); i++)
									{
										cout << i + 1 << ")";
										varsfunc.push_back(opzfunc.substr(indexchanged[i], 1));
										//varsfunc = findVarsFunc(indexchanged[i], opzfunc, opzformula1, start_index_formula);
										//varsformula1 = findVarsFormula(opzformula1);
										varsformula1.push_back(opzformula1.substr(0, 1));
										cout << "Соответствующие операнды в 1 части формулы и в выражении:" << endl;
										for (int j = 0; j < varsfunc.size(); j++)
											cout << varsformula1[j] << " - " << varsfunc[j] << endl;
										string opzfuncinsert;
										opzfuncinsert = findOpzFuncInsert(opzformula2, varsformula1, varsfunc);
										//cout << "  o p z funcinsert = " << opzfuncinsert << endl;
										opzfuncoutput = opzfunc;
										//cout << "opzfuncoutput = " << opzfuncoutput << endl;
										opzfuncoutput.erase(indexchanged[i], 1);
										//cout << "    after erase   opzfuncoutput = " << opzfuncoutput << endl;
										opzfuncoutput.insert(indexchanged[i], opzfuncinsert);
										//cout << "opzfuncoutput = " << opzfuncoutput << endl;
										funcoutput = opzfuncoutput;
										int temp = funcoutput.length() - 1;
										AssembleOperation(funcoutput, temp, temp);
										//cout << "funcoutput = " << funcoutput << endl;
										deleteExcessiveBrackets(funcoutput);
										cout << "Выражение                          = " << func << endl;
										cout << "Выражение после применения формулы = " << funcoutput << endl;
										varsformula1.clear();
										varsfunc.clear();
									}
								}
								else
								{
									indexchanged = findCorrectIndex(opzfunc, opzformula1, start_index_formula, index);
									if (indexchanged.size() != 0)
									{
										for (int i = 0; i < indexchanged.size(); i++)//Для каждого индекса найдем соответствующие массивы переменных(констант)
										{
											cout << i + 1 << ")";
											varsfunc = findVarsFunc(indexchanged[i], opzfunc, opzformula1, start_index_formula);
											varsformula1 = findVarsFormula(opzformula1);
											//	for (int j = 0; j < varsfunc.size(); j++)
											//		deleteExcessiveBrackets(varsfunc[j]);
											cout << "Соответствующие операнды в 1 части формулы и в выражении:" << endl;
											for (int j = 0; j < varsfunc.size(); j++)
												cout << varsformula1[j] << " - " << varsfunc[j] << endl;
											string opzfuncinsert;
											opzfuncinsert = findOpzFuncInsert(opzformula2, varsformula1, varsfunc);
											//cout << "  o p z funcinsert = " << opzfuncinsert << endl;
											opzfuncoutput = opzfunc;
											//cout << "opzfuncoutput = " << opzfuncoutput << endl;
											eraseFormulaInFunc(opzfunc, opzfuncoutput, opzformula1, opzfuncinsert, indexchanged[i], start_index_formula);
											//cout << "opzfuncoutput = " << opzfuncoutput << endl;
											funcoutput = opzfuncoutput;
											int temp = funcoutput.length() - 1;
											AssembleOperation(funcoutput, temp, temp);
											//cout << "funcoutput = " << funcoutput << endl;
											deleteExcessiveBrackets(funcoutput);
											//944 - голубой-черный, 244 - бело - красный, 240 - бело-черный, 15 - стандарт
											//SetConsoleTextAttribute(hConsole, 944);
											cout << "Выражение                          = " << func << endl;
											cout << "Выражение после применения формулы = " << funcoutput << endl;
											varsfunc.clear();
											varsformula1.clear();
										}
									}
									else
										cout << "Не было найдено место применения формулы к выражению." << endl;
								}
							}
							else
								cout << "Не было найдено место применения формулы к выражению." << endl;
							//cout << "formula1 = " << formula1 << endl;
							//cout << "formula2 = " << formula2 << endl;
							start_index_formula = 0;
							index.clear();
							indexchanged.clear();
							string temp;
							temp = opzformula1;
							opzformula1 = opzformula2;
							opzformula2 = temp;
							cout << "Формула = " << formula2 << "=" << formula1 << endl;
							index = findIndex(opzformula1, opzfunc, start_index_formula);
							//cout << "opzfunction: " << endl;
							//cout << opzfunc << endl;
							//cout << "opzformula1: " << endl;
							//cout << opzformula1 << endl;
							//cout << "opzformula2: " << endl;
							//cout << opzformula2 << endl;
							if (index.size() != 0)
							{
								if ((symboltype(opzfunc[index[0]]) == 5) || (symboltype(opzfunc[index[0]]) == 6))//Если в формуле одна константа или переменная
								{
									indexchanged = index;
									for (int i = 0; i < indexchanged.size(); i++)
									{
										cout << i + 1 << ")";
										varsfunc.push_back(opzfunc.substr(indexchanged[i], 1));
										//varsfunc = findVarsFunc(indexchanged[i], opzfunc, opzformula1, start_index_formula);
										//varsformula1 = findVarsFormula(opzformula1);
										varsformula1.push_back(opzformula1.substr(0, 1));
										cout << "Соответствующие операнды в 1 части формулы и в выражении:" << endl;
										for (int j = 0; j < varsfunc.size(); j++)
											cout << varsformula1[j] << " - " << varsfunc[j] << endl;
										string opzfuncinsert;
										opzfuncinsert = findOpzFuncInsert(opzformula2, varsformula1, varsfunc);
										//cout << "  o p z funcinsert = " << opzfuncinsert << endl;
										opzfuncoutput = opzfunc;
										//cout << "opzfuncoutput = " << opzfuncoutput << endl;
										opzfuncoutput.erase(indexchanged[i], 1);
										//cout << "    after erase   opzfuncoutput = " << opzfuncoutput << endl;
										opzfuncoutput.insert(indexchanged[i], opzfuncinsert);
										//cout << "opzfuncoutput = " << opzfuncoutput << endl;
										funcoutput = opzfuncoutput;
										int temp = funcoutput.length() - 1;
										AssembleOperation(funcoutput, temp, temp);
										//cout << "funcoutput = " << funcoutput << endl;
										deleteExcessiveBrackets(funcoutput);
										cout << "Выражение                          = " << func << endl;
										cout << "Выражение после применения формулы = " << funcoutput << endl;
										varsformula1.clear();
										varsfunc.clear();
									}
								}
								else
								{
									indexchanged = findCorrectIndex(opzfunc, opzformula1, start_index_formula, index);
									if (indexchanged.size() != 0)
									{
										for (int i = 0; i < indexchanged.size(); i++)//Для каждого индекса найдем соответствующие массивы переменных(констант)
										{
											cout << i + 1 << ")";
											varsfunc = findVarsFunc(indexchanged[i], opzfunc, opzformula1, start_index_formula);
											varsformula1 = findVarsFormula(opzformula1);
											//	for (int j = 0; j < varsfunc.size(); j++)
											//		deleteExcessiveBrackets(varsfunc[j]);
											cout << "Соответствующие операнды в 1 части формулы и в выражении:" << endl;
											for (int j = 0; j < varsfunc.size(); j++)
												cout << varsformula1[j] << " - " << varsfunc[j] << endl;
											string opzfuncinsert;
											opzfuncinsert = findOpzFuncInsert(opzformula2, varsformula1, varsfunc);
											//cout << "  o p z funcinsert = " << opzfuncinsert << endl;
											opzfuncoutput = opzfunc;
											//cout << "opzfuncoutput = " << opzfuncoutput << endl;
											eraseFormulaInFunc(opzfunc, opzfuncoutput, opzformula1, opzfuncinsert, indexchanged[i], start_index_formula);
											//cout << "opzfuncoutput = " << opzfuncoutput << endl;
											funcoutput = opzfuncoutput;
											int temp = funcoutput.length() - 1;
											AssembleOperation(funcoutput, temp, temp);
											//cout << "funcoutput = " << funcoutput << endl;
											deleteExcessiveBrackets(funcoutput);
											cout << "Выражение                          = " << func << endl;
											cout << "Выражение после применения формулы = " << funcoutput << endl;
											varsformula1.clear();
											varsfunc.clear();
										}
									}
									else
										cout << "Не было найдено место применения формулы к выражению." << endl;
								}
							}
							else
								cout << "Не было найдено место применения формулы к выражению." << endl;
						}
						else
							cout << "Формула неверна." << endl;
					}
					else
						cout << "Неверно введены исходные данные." << endl;
				}
				else
					cout << "Ошибка открытия файла!" << endl;
				cout << endl << "1.Выбор одного из применений формулы." << endl;
				cout << "2.Выбор другой формулы преобразования из имеющихся." << endl;
				cout << "3.Ввод новой формулы преобразования." << endl;
				cout << "4.Возврат к предыдущему изменению формулы." << endl;
				cout << "0.Завершение работы с выражением." << endl;
				cout << "Ваш выбор: " << endl;
				cin >> dec2;
				switch (dec2)
				{
				case 1:
					int first, second;
					cout << "Введите 2 числа через пробел или через enter." << endl;
					cout << "Первое число - 1 или 2 в зависимости от направления" << endl;
					cout << "применения формулы: 1, если слева направо(1 случай)," << endl;
					cout << "2, если справа налево(2 случай)." << endl;
					cout << "Второе число - номер по счету места применения операции." << endl;
					cin >> first >> second;
					if (findNLineInFile(func, "expression.txt", 1) && findNLineInFile(formula, "formula.txt", n))
					{
						if ((correctnessOfFormula(formula)) && (correctnessOfExpression(func)) && (checkFormulaVariables(formula, func)))
						{
							splitFormula(formula, formula1, formula2);
							opzfunc = exprToOpz(func);
							opzformula1 = exprToOpz(formula1);
							opzformula2 = exprToOpz(formula2);
							if (first == 2)
							{
								string temp;
								temp = opzformula1;
								opzformula1 = opzformula2;
								opzformula2 = temp;
							}
							if (truthTable(opzformula1, opzformula2))
							{
								int start_index_formula = 0;
								vector<int> index;
								vector<int> indexchanged;
								index = findIndex(opzformula1, opzfunc, start_index_formula);
								if (index.size() != 0)
								{
									if ((symboltype(opzfunc[index[0]]) == 5) || (symboltype(opzfunc[index[0]]) == 6))//Если в формуле одна константа или переменная
									{
										indexchanged = index;
										for (int i = 0; i < indexchanged.size(); i++)
										{
											if (i + 1 == second)
											{
												varsfunc.push_back(opzfunc.substr(indexchanged[i], 1));
												varsformula1.push_back(opzformula1.substr(0, 1));
												string opzfuncinsert;
												opzfuncinsert = findOpzFuncInsert(opzformula2, varsformula1, varsfunc);
												opzfuncoutput = opzfunc;
												opzfuncoutput.erase(indexchanged[i], 1);
												opzfuncoutput.insert(indexchanged[i], opzfuncinsert);
												funcoutput = opzfuncoutput;
												int temp = funcoutput.length() - 1;
												AssembleOperation(funcoutput, temp, temp);
												deleteExcessiveBrackets(funcoutput);
												result.push_back(func);
												printLineInEndOfFileWithClear(funcoutput, "expression.txt");
												varsformula1.clear();
												varsfunc.clear();
											}
										}
									}
									else
									{
										indexchanged = findCorrectIndex(opzfunc, opzformula1, start_index_formula, index);
										if (indexchanged.size() != 0)
										{
											for (int i = 0; i < indexchanged.size(); i++)//Для каждого индекса найдем соответствующие массивы переменных(констант)
											{
												if (i + 1 == second)
												{
													varsfunc = findVarsFunc(indexchanged[i], opzfunc, opzformula1, start_index_formula);
													varsformula1 = findVarsFormula(opzformula1);
													string opzfuncinsert;
													opzfuncinsert = findOpzFuncInsert(opzformula2, varsformula1, varsfunc);
													opzfuncoutput = opzfunc;
													eraseFormulaInFunc(opzfunc, opzfuncoutput, opzformula1, opzfuncinsert, indexchanged[i], start_index_formula);
													funcoutput = opzfuncoutput;
													int temp = funcoutput.length() - 1;
													AssembleOperation(funcoutput, temp, temp);
													deleteExcessiveBrackets(funcoutput);
													result.push_back(func);
													printLineInEndOfFileWithClear(funcoutput, "expression.txt");
													varsfunc.clear();
													varsformula1.clear();
												}
											}
										}
										else
											cout << "Не было найдено место применения формулы к выражению." << endl;
									}
								}
								else
									cout << "Не было найдено место применения формулы к выражению." << endl;
								
							}
							else
								cout << "Формула неверна." << endl;
						}
						else
							cout << "Неверно введены исходные данные." << endl;
					}
					else
						cout << "Ошибка открытия файла!" << endl;
					break;
				case 2:
					cout << "Имеющиеся формулы: " << endl;
					outContentsOfFile("formula.txt");
					cout << endl;
					cout << "Введите номер новой формулы: " << endl;
					cin >> n;
					break;
				case 3:
					cout << "Введите формулу для преобразования: " << endl;
					cin >> formula;
					if (correctnessOfFormula(formula))
					{
						splitFormula(formula, formula1, formula2);
						opzformula1 = exprToOpz(formula1);
						opzformula2 = exprToOpz(formula2);
						if (truthTable(opzformula1, opzformula2))
							printLineInEndOfFile(formula, "formula.txt");
						else
							cout << "Формула неверна!" << endl;
					}
					else
						cout << "Формула введена неверно!" << endl;
					break;
				case 4:
					if (!result.empty())
					{
						printLineInEndOfFileWithClear(result[result.size() - 1], "expression.txt");
						result.pop_back();
					}
					break;
				case 0:
					result.push_back(func);
					printLinesInEndOfFile(result, "log.txt");
					result.clear();
					n = 1;
					break;
				default:
					cout << "Пожалуйста, введите пункт меню!" << endl;
					break;
				}
			} while (dec2);
			break;
		case 2:
			system("cls");
			cout << "Введите выражение, которое собираетесь упрощать: " << endl;
			flag = 0;
			do
			{
				cin >> func;
				if (correctnessOfExpression(func))
				{
					printLineInEndOfFileWithClear(func, "expression.txt");
					flag = 1;
				}
				else
					cout << "Некорректно введено выражение!" << endl;
			} while (!flag);
			break;
		case 3:
			system("cls");
			if (outContentsOfFile("tasks.txt"))
			{
				do
				{
					cout << endl;
					cout << "1.Выбор одного из выражений для упрощения." << endl;
					cout << "0.Возврат в главное меню." << endl;
					cout << "Ваш выбор: " << endl;
					cin >> dec2;
					switch (dec2)
					{
					case 1:
						int n;
						cout << "Введите число - номер выражения для упрощения из списка выше: " << endl;
						cin >> n;
						findNLineInFile(func, "tasks.txt", n);
						if (correctnessOfExpression(func))
						{
							printLineInEndOfFileWithClear(func, "expression.txt");
							dec2 = 0;
							cout << "Нажмите 'Enter' для возврата в главное меню." << endl;
							getchar();
							getchar();
						}
						else
							cout << "Данное выражение некорректно!" << endl;
						break;
					case 0:
						cout << "Возврат в главное меню" << endl;
						break;
					default:
						cout << "Пожалуйста, введите один из пунктов подменю!" << endl;
					}
				} while (dec2);
			}
			else
			{
				cout << "Нажмите 'Enter' для возврата в главное меню." << endl;
				getchar();
				getchar();
			}
			break;
		case 4:
			do
			{
				system("cls");
				cout << "Имеющиеся формулы: " << endl;
				outContentsOfFile("formula.txt");
				cout << endl;
				cout << "1.Ввести новую формулу." << endl;
				cout << "2.Удалить все имеющиеся формулы." << endl;
				cout << "0.Вернуться в главное меню." << endl;
				cin >> dec2;
				switch (dec2)
				{
				case 1:
					cout << "Введите формулу для преобразования: " << endl;
					cin >> formula;
					if (correctnessOfFormula(formula))
					{
						splitFormula(formula, formula1, formula2);
						opzformula1 = exprToOpz(formula1);
						opzformula2 = exprToOpz(formula2);
						if (truthTable(opzformula1, opzformula2))
							printLineInEndOfFile(formula, "formula.txt");
						else
							cout << "Формула неверна!" << endl;
					}
					else
						cout << "Формула введена неверно!" << endl;
					break;
				case 2:
					clearFile("formula.txt");
					break;
				case 0:
					break;
				default:
					cout << "Пожалуйста, введите пункт меню!" << endl;
					break;
				}
			} while (dec2);
			break;
		case 5:
			system("cls");
			cout << "Данная программа рассчитана на работу с файлами." << endl;
			cout << "Необходимо создать в директории данного приложения 2 .txt файла." << endl;
			cout << "Название одного из них - formula.txt , второго - expression.txt" << endl;
			cout << "Создать и заполнить данные файлы можно и из интерфейса программы, в пунктах 2 и 3 меню." << endl;
			cout << "Также необходимо скачать файл tasks.txt с готовыми заданиями, либо создать его и записать туда" << endl;
			cout << "свои выражения для упрощения." << endl;
			cout << "В файл formula.txt запишите сколько угодно формул для преобразования(1 в каждую строку)" << endl;
			cout << "в виде выражение=выражение , не используя лишних пробелов." << endl;
			cout << "Следует всегда оставлять пустую строку в конце файла, чтобы в нее записывалась следующая формула." << endl;
			cout << "Важно! Переменные выражения и формул преобразования не должны совпадать, поэтому рекомендуется" << endl;
			cout << "при записи выражений использовать прописные символы, а при записи формул - строчные, или наоборот." << endl;
			cout << "В файл expression.txt запишите в первую строчку выражение, которое будете преобразовывать." << endl;
			cout << "Обратите внимание, что пункт 2 меню перезаписывает введенное ранее выражение для упрощения." << endl;
			cout << "После завершения работы с выражением(выхода из пункта 1 в главное меню) этапы Ваших преобразований" << endl;
			cout << "формулы будут записаны в файл под названием log.txt , находящийся в директории приложения." << endl;
			cout << "Очистить историю преобразований можно вручную либо выбрав 5 пункт меню." << endl;
			cout << "По умолчанию используется первая по счету формула в файле formula.txt" << endl << endl;
			cout << "Нажмите 'Enter', чтобы вернуться в главное меню" << endl;
			getchar();
			getchar();
			break;
		case 6:
			system("cls");
			cout << "История упрощений:" << endl;
			flag = outContentsOfFile("log.txt");
			if (flag == 0)
				cout << "Не удается открыть файл 'log.txt'!" << endl;
			cout << "\nНажмите 'Enter', чтобы вернуться в главное меню" << endl;
			getchar();
			getchar();
			break;
		case 7:
			clearFile("log.txt");
			break;
		case 0:
			cout << "Завершение работы программы." << endl;
			break;
		default:
			cout << "Пожалуйста, введите один из пунктов меню!" << endl;
			break;
		}
	} while (dec);
	cout << "\nНажмите 'Enter', чтобы выйти из программы" << endl;
	getchar();
	return 0;
}