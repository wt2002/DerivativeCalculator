#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <ctype.h>
#include <cmath>
#include <cstdlib>
#include <sstream>
using namespace std;

bool isNum(string);

string toStr(double num)
{
	stringstream ss;
	ss << num;
	string str;
	ss >> str;
	return str;
}

enum Type
{
	TOK_NUMBER = 255,
	TOK_NAME,
	TOK_ZUOKUOHAO,
	TOK_YOUKUOHAO,
	TOK_COMMA,
	TOK_OP
};

struct Token
{
	int type;
	string value;

	Token(int type, string value)
	{
		this->type = type;
		this->value = value;
	}

	Token(int type, char value)
	{
		string t;
		t += value;
		this->type = type;
		this->value = t;
	}
};

struct BTNode
{
	int type;
	string op;
	string value;
	BTNode* left;
	BTNode* right;

	BTNode(int type, string str)
	{
		this->type = type;
		if (type == TOK_OP)
			this->op = str;
		else this->value = str;
	}


	/*BTNode* calc()
	{
		if (this->op == "*")
		{
			//if(left->operator==(right))
		}
	}*/

	string toString()
	{
		if (type == TOK_NUMBER || type == TOK_NAME)
			return value;
		else if (op == "sin" || op == "cos" || op == "tan" || op == "ln")
			return op + "(" + left->toString() + ")";
		else if (op == "log")
			return "log(" + left->toString() + ", " + right->toString() + ")";
		else
		{
			if (op == "+")
			{
				if (left->toString() == "0")
					return right->toString();
				else if (right->toString() == "0")
					return left->toString();
				else if (left->toString() == "0" && right->toString() == "0")
					return "0";
				else
					return "(" + left->toString() + op + right->toString() + ")";
			}
			else if (op == "-")
			{
				if (left == nullptr)
					return "-" + right->toString();
				else if (left->toString() == "0")
					return "-" + right->toString();
				else if (right->toString() == "0")
					return left->toString();
				else if (left->toString() == "0" && right->toString() == "0")
					return "0";
				else
					return "(" + left->toString() + op + right->toString() + ")";
			}
			else if (op == "*")
			{
				if (left->toString() == "1")
					return right->toString();
				else if (right->toString() == "1")
					return left->toString();
				if (left->toString() == "0" || right->toString() == "0")
					return "0";
				else
					return "(" + left->toString() + op + right->toString() + ")";
			}
			else if (op == "/")
			{
				if (left->toString() == "0" || left->toString() == "-0")
					return "0";
				else
					return left->toString() + op + right->toString();
					//return "\\frac{" + left->toString() + "}{" + right->toString() + "}";
			}
			else if (op == "^")
			{
				if (left->toString() == "0")
					return "0";
				else if(right->toString() == "1")
					return left->toString();
				else if(right->toString() == "0")
					return "1";
				else
					return left->toString() + op + right->toString();
			}
				
		}
			
	}

	//??????
	BTNode* diff(string var)
	{
		//???????????????????????????????????????????????????0
		if ((type == TOK_NUMBER && isNum(value)) || (type == TOK_NAME && value != var))
			return new BTNode(TOK_NUMBER, "0");
		//???????????????????????????????????????1
		else if (type == TOK_NAME && value == var)
		{
			return new BTNode(TOK_NUMBER, "1");
		}
		//?????????
		else if (op == "^")
		{
			//x^2??????????????????????????????????????????????????????????????????
			if (left->value == var && right->type == TOK_NUMBER)
			{
				BTNode* n, * n2;
				n = new BTNode(TOK_OP, "^");
				n->left = new BTNode(TOK_NUMBER, var);
				n->right = new BTNode(TOK_NUMBER, toStr(atof(right->value.c_str()) - 1));
				n2 = new BTNode(TOK_OP, "*");
				n2->left = new BTNode(TOK_NUMBER, toStr(atof(right->value.c_str())));
				n2->right = n;
				return n2;
			}
			//x^k???x^(a+b)?????????
			else if (left->value == var && (right->type == TOK_NAME || right->type == TOK_OP))
			{
				BTNode* n, * n2;
				n = new BTNode(TOK_OP, "^");
				n->right = new BTNode(TOK_OP, "-");
				n->left = new BTNode(TOK_NAME, var);
				n->right->left = right;
				n->right->right = new BTNode(TOK_NUMBER, "1");
				n2 = new BTNode(TOK_OP, "*");
				n2->left = right;
				n2->right = n;
				return n2;
			}
			//(x+a)^2????????????
			else if (left->type == TOK_OP && right->type == TOK_NUMBER)
			{
				BTNode* n = new BTNode(TOK_OP, "*");
				BTNode* begin = n;
				n->right = left;

				//?????????????????????????????????????????????
				//TODO: ?????????????????????????????????????????????????????????
				int times = atoi(right->value.c_str());
				for (int i = 0; i < times - 2; i++)
				{
					begin->right = left;
					begin->left = new BTNode(TOK_OP, "*");
					begin = begin->left;
				}
				begin->right = left;
				begin->left = left;
				return n->diff(var);
			}
			//?????????????????????????????????????????????????????????????????????????????????????????????
			else if ((left->type == TOK_NUMBER || (left->type == TOK_NAME && left->value != var)) && (right->value == var || right->type == TOK_OP))
			{
				BTNode* n = new BTNode(TOK_OP, "*");
				BTNode* L = new BTNode(TOK_OP, "^");
				BTNode* R = new BTNode(TOK_OP, "ln");
				L->left = left;
				L->right = right;
				R->left = left;
				R->right = nullptr;
				n->left = L;
				n->right = R;
				return n;
			}
		}
		//???????????????
		else if (op == "+")
		{
			BTNode* n = new BTNode(TOK_OP, "+");
			n->left = this->left->diff(var);
			n->right = this->right->diff(var);
			return n;
		}
		//???????????????????????????????????????
		else if (op == "-")
		{
			if (left != nullptr)
			{
				BTNode* n = new BTNode(TOK_OP, "-");
				n->left = this->left->diff(var);
				n->right = this->right->diff(var);
				return n;
			}
			else
			{
				BTNode* n = new BTNode(TOK_OP, "-");
				n->left = new BTNode(TOK_NUMBER, "0");
				n->right = this->right->diff(var);
				return n;
			}
		}
		//????????????
		else if (op == "*")
		{
			BTNode* n = new BTNode(TOK_OP, "+");
			BTNode* L = new BTNode(TOK_OP, "*");
			BTNode* R = new BTNode(TOK_OP, "*");
			L->left = left->diff(var);
			L->right = right;
			R->left = left;
			R->right = right->diff(var);
			n->left = L;
			n->right = R;
			return n;
		}
		//????????????
		else if (op == "/")
		{
			BTNode* n = new BTNode(TOK_OP, "/");
			BTNode* d = new BTNode(TOK_OP, "-");
			BTNode* L = new BTNode(TOK_OP, "*");
			BTNode* R = new BTNode(TOK_OP, "*");
			BTNode* p = new BTNode(TOK_OP, "^");
			L->left = left->diff(var);
			L->right = right;
			R->left = left;
			R->right = right->diff(var);
			d->left = L;
			d->right = R;
			p->left = right;
			p->right = new BTNode(TOK_NUMBER, "2");
			n->left = d;
			n->right = p;
			return n;
		}
		//???????????????
		else if (op == "log")
		{
			BTNode* result = new BTNode(TOK_OP, "*");
			BTNode* n = new BTNode(TOK_OP, "/");
			BTNode* L = new BTNode(TOK_OP, "ln");
			BTNode* R = new BTNode(TOK_OP, "*");
			L->left = left;
			L->right = nullptr;
			R->left = right;
			R->right = L;
			n->left = new BTNode(TOK_NUMBER, "1");
			n->right = R;
			result->left = n;
			result->right = right->diff(var);
			return result;
		}
		//?????????????????????
		else if (op == "ln")
		{
			BTNode* result = new BTNode(TOK_OP, "*");
			BTNode* n = new BTNode(TOK_OP, "/");
			n->left = new BTNode(TOK_NUMBER, "1");
			n->right = left;
			result->left = n;
			result->right = left ->diff(var);
			return result;
		}
		//????????????????????????
		else if (op == "sin")
		{
			BTNode* result = new BTNode(TOK_OP, "*");
			BTNode* n = new BTNode(TOK_OP, "cos");
			n->left = left;
			n->right = nullptr;
			result->left = n;
			result->right = left->diff(var);
			return result;
		}
		//????????????????????????
		else if (op == "cos")
		{
			BTNode* result = new BTNode(TOK_OP, "*");
			BTNode* m = new BTNode(TOK_OP, "-");
			BTNode* n = new BTNode(TOK_OP, "sin");
			n->left = left;
			n->right = nullptr;
			m->left = new BTNode(TOK_NUMBER, "0");
			m->right = n;
			result->left = m;
			result->right = left->diff(var);
			return result;
		}
		//???????????????????????????????????????sin(...)/cos(...)?????????
		else if (op == "tan")
		{
			BTNode* n = new BTNode(TOK_OP, "/");
			BTNode* s = new BTNode(TOK_OP, "sin");
			BTNode* c = new BTNode(TOK_OP, "cos");
			s->left = left;
			s->right = nullptr;
			c->left = left;
			c->right = nullptr;
			n->left = s;
			n->right = c;
			return n->diff(var);
		}
	}
};

bool isNum(string str)
{
	int r = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] >= '0' && str[i] <= '9' || str[i] == '.')
			r += 0;
		else
			r = 1;
	}
	return r == 0;
}
vector<Token*> tokens;//????????????
vector<Token> suffixExp;//???????????????

//?????????????????????
int inline opLevel(Token op)
{
	if (op.value == "+" || op.value == "-")
		return 0;
	else if (op.value == "*" || op.value == "/")
		return 1;
	else if (op.value == "^")
		return 2;
	else if (op.type == TOK_NAME)
		return 3;
	else return -1;
}

//?????????????????????????????????
void parseExp(string exp)
{
	string str;
	int i = 0;
	while (i < exp.length())
	{
		str = "";
		char ch = exp[i];
		switch (ch)
		{
		case ' ':
			break;
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
			tokens.push_back(new Token(TOK_OP, ch));
			i++;
			break;
		case '(':
			tokens.push_back(new Token(TOK_ZUOKUOHAO, "("));
			i++;
			break;
		case ')':
			tokens.push_back(new Token(TOK_YOUKUOHAO, ")"));
			i++;
			break;
		case ',':
			tokens.push_back(new Token(TOK_COMMA, ","));
			i++;
			break;
		default:
			break;
		}

		if (isalpha(exp[i]) || exp[i] == '_')
		{
			while (i < exp.size() && isalpha(exp[i]) || exp[i] == '_' || (exp[i] >= '0' && exp[i] <= '9'))
				str += exp[i++];
			tokens.push_back(new Token(TOK_NAME, str));
		}
		else if (isdigit(exp[i]) || exp[i] == '-')
		{
			while (i < exp.size() && isdigit(exp[i]) || exp[i] == '.')
				str += exp[i++];
			tokens.push_back(new Token(TOK_NUMBER, str));
		}
	}
}

//????????????????????????
void getSuffixExp()
{
	//?????????
	stack<Token> ops;
	for (int i = 0; i < tokens.size(); i++)
	{
		Token t = *tokens[i];
		if (t.type == TOK_COMMA)
		{
			continue;
		}
		else if (t.type == TOK_NUMBER)
		{
			suffixExp.push_back(t);
		}
		else if (t.type == TOK_NAME)
		{
			if (i + 1 < tokens.size() && tokens[i + 1]->type == TOK_ZUOKUOHAO)
			{
				while (!ops.empty() && opLevel(ops.top()) >= opLevel(t) && ops.top().type != TOK_ZUOKUOHAO)
				{
					Token n = ops.top();
					n.type = TOK_OP;
					suffixExp.push_back(n);
					ops.pop();
				}
				ops.push(t);
			}
			else
			{
				suffixExp.push_back(t);
			}
		}
		else if (t.type == TOK_OP)
		{
			while (!ops.empty() && opLevel(ops.top()) >= opLevel(t) && ops.top().type != TOK_ZUOKUOHAO)
			{
				suffixExp.push_back(ops.top());
				ops.pop();
			}
			ops.push(t);
		}
		else if (t.type == TOK_YOUKUOHAO)
		{
			while (ops.top().type != TOK_ZUOKUOHAO)
			{
				suffixExp.push_back(ops.top());
				ops.pop();
			}
			ops.pop();

		}
		else if (t.type == TOK_ZUOKUOHAO)
		{
			ops.push(t);
		}
	}
	while (!ops.empty())
	{
		suffixExp.push_back(ops.top());
		ops.pop();
	}
}

//????????????name?????????????????????
bool isFuncName(string name)
{
	if (name == "sin" || name == "cos" || name == "tan" || name == "sqrt" || name == "ln" || name == "log")
		return true;
	else return false;
}

//??????????????????
stack<BTNode*> s;
void getBinaryTree()
{
	for (vector<Token>::iterator it = suffixExp.begin(); it != suffixExp.end(); it++)
	{
		if ((*it).type == TOK_NUMBER )
		{
			s.push(new BTNode(TOK_NUMBER, (*it).value));
		}
		else if ((*it).type == TOK_NAME && !isFuncName((*it).value))
		{
			s.push(new BTNode(TOK_NAME, (*it).value));
		}
		else
		{
			//??????????????????????????????log????????????????????????????????????????????????
			if (isFuncName((*it).value) && (*it).value != "log")
			{
				BTNode* n = s.top(); s.pop();
				BTNode* node = new BTNode(TOK_OP, (*it).value);
				node->left = n;
				node->right = nullptr;
				s.push(node);
			}
			else
			{
			//???????????????
				BTNode* r = s.top(); s.pop();
				BTNode* l = s.top(); s.pop();
				BTNode* node = new BTNode(TOK_OP, (*it).value);
				node->left = l;
				node->right = r;
				s.push(node);
			}
		}
	}
}
int main()
{
	string str;
	BTNode* expTree;
	cin >> str;
	//???????????????
	parseExp(str);
	//????????????????????????
	getSuffixExp();
	//????????????????????????????????????
	getBinaryTree();
	expTree = s.top();
	//??????
	BTNode *n = expTree->diff("x");
	cout << n->toString() << endl;
	return 0;
}