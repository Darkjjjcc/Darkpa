#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>


/* PA1.2 表达式求值_task1: 补充token类型*/
enum {
  TK_NOTYPE = 256,    // spaces
  TK_EQ,              // equal
  TK_NEQ,             // not equal
  TK_NUM,             // number
  TK_HEX_NUM,         // hex number
  TK_AND,             // and
  TK_OR,              // or
  TK_REG,             // register


  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  /* PA1.2 表达式求值_task2: 补充规则,词法分析
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\*", '*'},         // multiply
  {"\\/", '/'},         // divide
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // minus
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},       // not equal
  {"0x[0-9a-fA-F]+", TK_HEX_NUM}, // hex number
  {"[0-9]+", TK_NUM},   // number
  {"\\(", '('},         // left bracket
  {"\\)", ')'},         // right bracket
  {"&&", TK_AND},       // and
  {"\\|\\|", TK_OR},    // or
  {"\\$[a-zA-Z]+", TK_REG}, // register

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        /* PA1.2 表达式求值_task3: 补充token类型
         * tokens数组中的每一个元素都是一个token结构体，包含两个成员：
         * 1. type: 表示token的类型，是一个整数，可以是上面定义的TK_NOTYPE、TK_EQ等
         * 2. str: 表示token的字符串，是一个字符数组，用于存储token的字符串
        */
        switch (rules[i].token_type) {
		      TK_NOTYPE:break;
          case TK_NOTYPE:break;
          case TK_NUM: 
          {
            if (substr_len>32) { puts("The length of number is too long!"); return false; }
			      tokens[nr_token].type='0';
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len]='\0';
            ++nr_token;
            break;
          }
          case TK_HEX_NUM:
          {
          if (substr_len>32) { puts("The length of number is too long!"); return false; }
            tokens[nr_token].type='6';
            strncpy(tokens[nr_token].str,substr_start+2,substr_len-2);
            tokens[nr_token].str[substr_len-2]='\0';
            ++nr_token;
            break;
          }

          // fractions
          case '(':
          case ')':

          // operators
          case '*':
          case '/':
          case '-':
          case '+':
          case TK_EQ:
          case TK_NEQ:
          case TK_AND:
          case TK_OR:{tokens[nr_token++].type=rules[i].token_type;break;}

          case TK_REG:
          {
            tokens[nr_token].type='r';  
            strncpy(tokens[nr_token].str,substr_start+1,substr_len-1);
            tokens[nr_token].str[substr_len-1]='\0';
            ++nr_token;
            break;
          }
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t check_parentheses(int start, int end){
  if (tokens[start].type != '(' || tokens[end].type != ')') {
    return false;
  }
  int i, cnt = 0;
  for (i = start + 1; i < end; i++) {
    if (tokens[i].type == '(') {
      cnt++;
    }
    else if (tokens[i].type == ')') {
      cnt--;
    }
    if (cnt < 0) {
      return false;
    }
  }
  return true;
}

uint32_t operator_priority(int type) {
  switch (type) {
    case TK_OR: return 1;
    case TK_AND: return 2;
    case TK_EQ: case TK_NEQ: return 3;
    case '+': case '-': return 4;
    case '*': case '/': return 5;
    case '(': return 6;
    default: return 0;
  }
}

uint32_t main_operator(int start, int end) {
  int i, cnt = 0, main_op = -1;
  uint32_t main_pri = 10;
  for (i = start; i <= end; i++) {
    if (tokens[i].type == '(') {
      cnt++;
    }
    else if (tokens[i].type == ')') {
      cnt--;
    }
    if (cnt == 0) {
      if (tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' || tokens[i].type == '/' 
                || tokens[i].type == TK_EQ || tokens[i].type == TK_NEQ || tokens[i].type == TK_AND || tokens[i].type == TK_OR) 
      {
        if (operator_priority(tokens[i].type) < main_pri) {
          main_pri = operator_priority(tokens[i].type);
          main_op = i;
        }
      }
    }
  }
  return main_op;
}

uint32_t eval(int start, int end, bool *success) {
  if (start > end) {
    /* Bad expression */
    *success = false;
    return 0;
  }
  else if (start == end) {
    // Single token.
    if(tokens[start].type == '0') return atoi(tokens[start].str);
    else if(tokens[start].type == '6') return strtol(tokens[start].str,NULL,16);
    else if(tokens[start].type == 'r') return isa_reg_str2val(tokens[start].str+1,success);
    else assert(0);
  }
  else if (check_parentheses(start, end) == true) {
    // The expression is surrounded by a matched pair of parentheses.
    return eval(start + 1, end - 1, success);
  }
  else {
    // Find the dominant operator in the token expression.
    int op = main_operator(start, end);
    if (op == -1) {
      // printf("no main operator found\n");
      *success = false;
      return 0;
    }
    uint32_t val1 = eval(start, op - 1, success);
    uint32_t val2 = eval(op + 1, end, success);
    if (*success == false) {
      return 0;
    }
    // printf("now operator is %c\n", tokens[op].type);
    switch (tokens[op].type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      case TK_EQ: return val1 == val2;
      case TK_NEQ: return val1 != val2;
      case TK_AND: return val1 && val2;
      case TK_OR: return val1 || val2;
      default: assert(0);
    }
    
  }
}



uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  *success = true;
  return eval(0, nr_token - 1, success);

  return 0;
}
