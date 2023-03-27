/**
name: Mariano Garcia
project: CSE 324 Lisp-Interpreter
*/

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <cmath>

/* cell */
enum ctype {
	Lambda, List, Number, Procedure, Symbol
};

struct environment;

struct cell {
	typedef cell (*ptype)(const std::vector<cell> &);
	typedef std::vector<cell>::const_iterator citer1;
	typedef std::map<std::string, cell> map;
	ctype type;
	std::string val;
	std::vector<cell> list;
	ptype proc;
	environment *enviro;
	cell(ctype type = Symbol) : type(type), enviro(0) {}
	cell(ctype type, const std::string & val) : type(type), val(val), enviro(0) {}
	cell(ptype proc) : type(Procedure), proc(proc), enviro(0) {}
};

typedef std::vector<cell> cells;
typedef cells::const_iterator citer2;
const cell iffalse(Symbol, "#f");
const cell iftrue(Symbol, "#t");
const cell nil(Symbol, "nil");

/**
 * Converts the numbers to a string
 * @param  a               the string
 * @return   String version
 */
std::string str(long input) {
	std::ostringstream os;
	os << input;
	return os.str();
}

bool chkdig(char c) {
	return isdigit(static_cast <unsigned char> (c)) != 0;
}

/**
 * The enviroment being set up
 */
struct environment {
	environment(environment * out = 0):out_(out) {}

	environment(const cells & params, const cells & args, environment * out):out_(out) {
		citer2 x = args.begin();
		for(citer2 p = params.begin(); p != params.end(); ++p)
			enviro_[p->val] = *x++;
	}

	typedef std::map<std::string, cell>map;
	map & find(const std::string & var) {
		if(enviro_.find(var) != enviro_.end()) {return enviro_;}
		if(out_) {return out_->find(var);}
		std::cout << "An object cannot start with '" << var << "'\n";
		exit(1);
	}

	cell & operator[] (const std::string & var) {
		return enviro_[var];
	}
	private:
		map enviro_;
		environment * out_;
};

/**
 * Will add the two numbers together
 * @return   returns the sum
 */
cell addition(const cells & c) {
	long sum(atol(c[0].val.c_str()));

	for (citer2 i = c.begin()+1; i != c.end(); ++i){
		 sum += atol(i->val.c_str());
	}
	return cell(Number, str(sum));
}
/**
 * Will subtract the numbers
 * @return   returns the difference
 */
cell subtraction(const cells & c) {
	long x(atol(c[0].val.c_str()));
	for(citer2 i = c.begin()+1; i != c.end(); ++i){
		x -= atol(i->val.c_str());
	}
	return cell(Number, str(x));
}
/**
 * Will produce the product
 * @return   returns the product
 */
cell multiplication(const cells & c) {
	long product(1);
	for(citer2 i = c.begin(); i != c.end(); ++i){
		product *= atol(i->val.c_str());
	}
	return cell(Number, str(product));
}
/**
 * Will divide the numbers
 * @return   returns the divisor between the numbers
 */
cell division(const cells & c) {
	long x(atol(c[0].val.c_str()));
	for(citer2 i = c.begin()+1; i != c.end(); ++i){
		x /= atol(i->val.c_str());
	}
	return cell(Number, str(x));
}
/**
 * Returns the squareroot of the numbers
 * @return   returns the squareroot
 */
cell sqrt(const cells & c) {
	long x(atol(c[0].val.c_str()));
	x = sqrt(x);
	return cell(Number, str(x));
}
/**
 * get the power of x to the power of y
 */
cell pow(const cells & c) {
	long x(atol(c[0].val.c_str()));
	long y(atol(c[0].val.c_str()));

	int count = 1;
	for(citer2 i = c.begin()+1; i != c.end(); ++i){
		while(count < atol(i->val.c_str())){
			x *= y;
			count++;
		}
	}
	return cell(Number, str(x));
}

cell greaterthan(const cells & c) {
	long x(atol(c[0].val.c_str()));
	for(citer2 i = c.begin()+1; i != c.end(); ++i){
			if(x <= atol(i->val.c_str())) {
				return iffalse;
			}
	}
	return iftrue;
}

cell lessthan(const cells & c) {
	long x(atol(c[0].val.c_str()));
	for(citer2 i = c.begin()+1; i != c.end(); ++i){
		if(x >= atol(i->val.c_str())) {
			return iffalse;
		}
	}

	return iftrue;
}

cell lesseq(const cells & c) {
	long x(atol(c[0].val.c_str()));
	for(citer2 i = c.begin()+1; i != c.end(); ++i){
		if(x > atol(i->val.c_str())) {
			return iffalse;
		}
	}

	return iftrue;
}

cell length(const cells & c) {
	return cell(Number, str(c[0].list.size()));
}

cell nullp(const cells & c) {
	return c[0].list.empty() ? iftrue : iffalse;
}

cell car(const cells & c) {
	return c[0].list[0];
}

cell cdr(const cells & c) {
	if(c[0].list.size() < 2) {
		return nil;
	}
	cell result(c[0]);
	result.list.erase(result.list.begin());
	return result;
}

cell append(const cells & c) {
	cell result(List);
	result.list = c[0].list;
	for(citer2 i = c[1].list.begin(); i != c[1].list.end(); ++i){
		result.list.push_back(*i);
	}
	return result;
}

cell construct(const cells & c) {
	cell result(List);
	result.list.push_back(c[0]);
	for(citer2 i = c[1].list.begin(); i != c[1].list.end(); ++i){
		result.list.push_back(*i);
	}
	return result;
}

cell list(const cells & c) {
	cell result(List);
	result.list = c;
	return result;
}


void prims(environment & enviro) {
	enviro["+"] = cell(&addition);
	enviro["-"] = cell(&subtraction);
	enviro["*"] = cell(&multiplication);
	enviro["/"] = cell(&division);
	enviro[">"] = cell(&greaterthan);
	enviro["<"] = cell(&lessthan);
	enviro["<="] = cell(&lesseq);
	enviro["append"] = cell(&append);
	enviro["car"] = cell(&car);
	enviro["cdr"] = cell(&cdr);
	enviro["cons"] = cell(&construct);
	enviro["length"] = cell(&length);
	enviro["list"] = cell(&list);
	enviro["null"] = cell(&nullp);
	enviro["nil"] = nil;
	enviro["#f"] = iffalse;
	enviro["#t"] = iftrue;
	enviro["sqrt"] = cell(&sqrt);
	enviro["pow"] = cell(&pow);
}

/**
 * Evaluates the input
 * @param  x                  Cell
 * @param  enviro               The inviroment
 * @return      Checks the return to see if it is valid or not
 */
cell eval(cell x, environment * enviro) {

    if(x.type == Symbol) {
		return enviro->find(x.val)[x.val];
	}
    if(x.type == Number) {
		return x;
	}
    if (x.list.empty()) {
		return nil;
	}
    if (x.list[0].type == Symbol) {
        if (x.list[0].val == "quote") {
			return x.list[1];
		}
        if (x.list[0].val == "if") {
			return eval(eval(x.list[1], enviro).val == "#f" ? (x.list.size() < 4 ? nil : x.list[3]) : x.list[2], enviro);
		}
        if (x.list[0].val == "set!") {
			return enviro->find(x.list[1].val)[x.list[1].val] = eval(x.list[2], enviro);
		}
        if (x.list[0].val == "define") {
			return (*enviro)[x.list[1].val] = eval(x.list[2], enviro);
		}
        if (x.list[0].val == "lambda") {
            x.type = Lambda;
            x.enviro = enviro;
            return x;
        }
		if (x.list[0].val == "and") {
			printf("Did not impliment\n");
		}
		if (x.list[0].val == "or") {
			printf("Did not impliment\n");
		}
		if (x.list[0].val == "not") {
			if (x.list[0].val == "nil") {
				printf("true");
				return x;
			}else{
				return x;
			}
		}
        if (x.list[0].val == "begin") {
            for (size_t i = 1; i < x.list.size() - 1; ++i)
                eval(x.list[i], enviro);
            return eval(x.list[x.list.size() - 1], enviro);
        }
    }
    cell proc(eval(x.list[0], enviro));
    cells exps;
    for (cell::citer1 exp = x.list.begin() + 1; exp != x.list.end(); ++exp)
        exps.push_back(eval(*exp, enviro));
    if (proc.type == Lambda) {
		return eval(proc.list[2], new environment(proc.list[1].list, exps, proc.enviro));
	}
    else if (proc.type == Procedure) {
		return proc.proc(exps);
	}
    std::cout << "Input isn't a function. Please try again and use a valid symbol\n";
    exit(1);
}

/**
 * Will parse the information given
 */
std::list<std::string> tokenize(const std::string & str) {
	std::list<std::string> tok;
	const char * a = str.c_str();
	while(*a) {
		while(*a == ' ')
			++a;
		if(*a == '(' || *a == ')')
			tok.push_back(*a++ == '(' ? "(" : ")");
		else {
			const char *b = a;
			while(*b && *b != ' ' && *b != '(' && *b != ')')
				++b;
			tok.push_back(std::string(a, b));
			a = b;
		}
	}
	return tok;
}

/**
 * The Atom
 */
cell atom(const std::string & token) {
	if(chkdig(token[0]) || (token[0] == '-' && chkdig(token[1]))) {return cell(Number, token);}
	return cell(Symbol, token);
}


/**
 * Will give you the LISP output
 */
cell readin(std::list<std::string> & toks) {
    const std::string token(toks.front());
	toks.pop_front();
	if(token == "(") {
		cell c(List);
		while(toks.front() != ")")
			c.list.push_back(readin(toks));
		toks.pop_front();
		return c;
	}
	else {return atom(token);}
}

cell intr(const std::string & s) {
	std::list<std::string> tok(tokenize(s));
	return readin(tok);
}

std::string lispstring(const cell & expression) {
	if(expression.type == List) {
		std::string s("(");
		for(cell::citer1 e = expression.list.begin(); e != expression.list.end(); ++e)
			s += lispstring(*e) + ' ';
		if(s[s.size() - 1] == ' ')
			s.erase(s.size() - 1);
		return s + ')';
	}
	else if(expression.type == Lambda) {return "<Lambda>";}
	else if(expression.type == Procedure) {return "<Procedure>";}
	return expression.val;
}
void prompt(const std::string & prompt, environment * enviro) {
	while(true) {
		std::cout << prompt;
		std::string line;
		std::getline(std::cin, line);
		std::cout << lispstring(eval(intr(line), enviro)) << '\n';
	}
}

int main() {
	environment enviro;

	prims(enviro);
	std::cout << "Lisp Interpretor has started\n";
	prompt(">", &enviro);
}
