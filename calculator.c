#include "calculator.h"
void stackInit(Stack *s) {
    s->content = NULL;
    s->size = 0;
    s->top = -1; }
void stackPush(Stack *s, void* val) {
    klee_detect_int(s->top, 1, 1);
    if(s->top + 1 >= s->size) {
        klee_detect_int(s->size, 1, 1); (s->size)++;
        klee_detect_int(s->size,sizeof(void*),4); s->content = (void**)realloc(s->content, s->size * sizeof(void*)); }
    (s->top)++;
    s->content[s->top] = val; }
void* stackTop(Stack *s) {
    void *ret = NULL;
    if(s->top >= 0 && s->content != NULL)
        ret = s->content[s->top];
    return ret; }
void* stackPop(Stack *s) {
    void *ret = NULL;
    if(s->top >= 0 && s->content != NULL)
        ret = s->content[(s->top)--];
    return ret; }
int stackSize(Stack *s) {
    klee_detect_int(s->top, 1, 1);
    return s->top + 1; }
void stackFree(Stack *s) {
    free(s->content);
    s->content = NULL;
    s->size = 0;
    s->top = -1; }
void raise(Error err) {
    char* msg;
    switch(err) {
    case divZero:
        msg = "Divide by zero";
        break;
    case overflow:
        msg = "Overflow";
        break;
    case parenMismatch:
        msg = "Mismatched parentheses";
        break; }
    printf("\tError: %s\n", msg); }
inline unsigned int toDigit(char ch) {
    return ch - '0'; }
number buildNumber(token str) {
    number result = 0;
    result = atoi(str);
    return result; }
token num2Str(number num) {
    token str = (token)malloc(20*num*sizeof(char));
    snprintf(str, 19, "%d", num);
    return str; }
inline number toRadians(number degrees) {
    return degrees * PI / 180.0; }
inline number toDegrees(number radians) {
    return radians * 180.0 / PI; }
token doFunc(token input, token function) {
    number num = buildNumber(input);
    number result = num;
    if(strcmp(function, "abs") == 0)
        result = fabs(num);
    else if(strcmp(function, "floor") == 0)
        result = floor(num);
    else if(strcmp(function, "ceil") == 0)
        result = ceil(num);
    else if(strcmp(function, "sin") == 0)
        result = !prefs.mode.degrees ? sin(num) : sin(toRadians(num));
    else if(strcmp(function, "cos") == 0)
        result = !prefs.mode.degrees ? cos(num) : cos(toRadians(num));
    else if(strcmp(function, "tan") == 0)
        result = !prefs.mode.degrees ? tan(num) : tan(toRadians(num));
    else if(strcmp(function, "arcsin") == 0
            || strcmp(function, "asin") == 0)
        result = !prefs.mode.degrees ? asin(num) : toDegrees(asin(num));
    else if(strcmp(function, "arccos") == 0
            || strcmp(function, "acos") == 0)
        result = !prefs.mode.degrees ? acos(num) : toDegrees(acos(num));
    else if(strcmp(function, "arctan") == 0
            || strcmp(function, "atan") == 0)
        result = !prefs.mode.degrees ? atan(num) : toDegrees(atan(num));
    else if(strcmp(function, "sqrt") == 0)
        result = sqrt(num);
    else if(strcmp(function, "cbrt") == 0)
        result = cbrt(num);
    else if(strcmp(function, "log") == 0)
        result = log(num);
    else if(strcmp(function, "exp") == 0)
        result = exp(num);
    return num2Str(result); }
token doOp(token loperand, token op, token roperand) {
    number lside = buildNumber(loperand);
    number rside = buildNumber(roperand);
    number ret;
    switch(*op) {
    case '^': {
        ret = pow(lside, rside); }
    break;
    case '*': {
        ret = lside * rside; }
    break;
    case '/': {
        if(rside == 0)
            raise(divZero);
        else
            ret = lside / rside; }
    break;
    case '%': {
        if(rside == 0)
            raise(divZero);
        else {
            ret = (int)(lside / rside);
            ret = lside - (ret * rside); } }
    break;
    case '+': {
        ret = lside + rside; }
    break;
    case '-': {
        ret = lside - rside; }
    break; }
    return num2Str(ret); }
char* ufgets(FILE* stream) {
    unsigned int maxlen = 128, size = 128;
    char* buffer = (char*)malloc(maxlen);
    if(buffer != NULL) {
        char ch = EOF;
        int pos = 0;

        while((ch = getchar()) != EOF && ch != '\n') {
            buffer[pos++] = ch;
            if(pos == size) {
                size = pos + maxlen;
                buffer = (char*)realloc(buffer, size); } }
        buffer[pos] = '\0'; }
    return buffer; }
Symbol type(char ch) {
    Symbol result;
    switch(ch) {
    case '+':
    case '-':
        result = addop;
        break;
    case '*':
    case '/':
    case '%':
        result = multop;
        break;
    case '^':
        result = expop;
        break;
    case '(':
        result = lparen;
        break;
    case ')':
        result = rparen;
        break;
    case '.':
        result = decimal;
        break;
    case ' ':
        result = space;
        break;
    case ',':
        result = argsep;
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        result = digit;
        break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
        result = text;
        break;
    default:
        result = invalid;
        break; }
    return result; }
bool isFunction(token tk) {
    return (strcmp(tk, "abs") == 0
            || strcmp(tk, "floor") == 0
            || strcmp(tk, "ceil") == 0
            || strcmp(tk, "sin") == 0
            || strcmp(tk, "cos") == 0
            || strcmp(tk, "tan") == 0
            || strcmp(tk, "arcsin") == 0
            || strcmp(tk, "arccos") == 0
            || strcmp(tk, "arctan") == 0
            || strcmp(tk, "asin") == 0
            || strcmp(tk, "acos") == 0
            || strcmp(tk, "atan") == 0
            || strcmp(tk, "sqrt") == 0
            || strcmp(tk, "cbrt") == 0
            || strcmp(tk, "log") == 0
            || strcmp(tk, "exp") == 0); }
Symbol tokenType(token tk) {
    Symbol ret = type(*tk);
    switch(ret) {
    case text:
        if(isFunction(tk))
            ret = function;
        else
            ret = identifier;
        break;
    case addop:
        if(*tk == '-' && strlen(tk) > 1)
            ret = tokenType(tk+1);
        break;
    case decimal:
    case digit:
        ret = value;
        break; }
    return ret; }
int tokenize(char *str, char *(**tokensRef)) {
    char** tokens = NULL;
    char* ptr = str;
    char ch = '\0';
    int numTokens = 0;
    while(ch = *ptr++) {
        if(type(ch) == invalid)
            break;
        token newToken = NULL;
        switch(type(ch)) {
        case addop: {
            if(ch == '-'
                    && (numTokens == 0
                        || (tokenType(tokens[numTokens-1]) == addop
                            || tokenType(tokens[numTokens-1]) == multop
                            || tokenType(tokens[numTokens-1]) == expop
                            || tokenType(tokens[numTokens-1]) == lparen))) {
                {
                    int len = 1;
                    bool hasDecimal = false;
                    bool hasExponent = false;
                    if(type(ch) == decimal) {
                        hasDecimal = true;
                        len++;
                        newToken = (char*)malloc((len + 1) * sizeof(char));
                        newToken[0] = '0';
                        newToken[1] = '.'; }
                    else {
                        newToken = (char*)malloc((len + 1) * sizeof(char));
                        newToken[len-1] = ch; }
                    for(;
                            *ptr
                            && (type(*ptr) == digit
                                || ((type(*ptr) == decimal
                                     && hasDecimal == 0))
                                || ((*ptr == 'E' || *ptr == 'e')
                                    && hasExponent == false));
                            ++len) {
                        if(type(*ptr) == decimal)
                            hasDecimal = true;
                        else if(*ptr == 'E' || *ptr == 'e')
                            hasExponent = true;
                        newToken = (char*)realloc(newToken, (len + 1) * sizeof(char));
                        newToken[len] = *ptr++; }
                    newToken[len] = '\0'; }
                break; } }
        case multop:
        case expop:
        case lparen:
        case rparen:
        case argsep: {
            newToken = (char*)malloc(2 * sizeof(char));
            newToken[0] = ch;
            newToken[1] = '\0'; }
        break;
        case digit:
        case decimal: {
            int len = 1;
            bool hasDecimal = false;
            bool hasExponent = false;
            if(type(ch) == decimal) {
                hasDecimal = true;
                len++;
                newToken = (char*)malloc((len + 1) * sizeof(char));
                newToken[0] = '0';
                newToken[1] = '.'; }
            else {
                newToken = (char*)malloc((len + 1) * sizeof(char));
                newToken[len-1] = ch; }
            for(;
                    *ptr
                    && (type(*ptr) == digit
                        || ((type(*ptr) == decimal
                             && hasDecimal == 0))
                        || ((*ptr == 'E' || *ptr == 'e')
                            && hasExponent == false));
                    ++len) {
                if(type(*ptr) == decimal)
                    hasDecimal = true;
                else if(*ptr == 'E' || *ptr == 'e')
                    hasExponent = true;
                newToken = (char*)realloc(newToken, (len + 1) * sizeof(char));
                newToken[len] = *ptr++; }
            newToken[len] = '\0'; }
        break;
        case text: {
            int len = 1;
            newToken = (char*)malloc((len + 1) * sizeof(char));
            newToken[0] = ch;
            for(len = 1; *ptr && type(*ptr) == text; ++len) {
                newToken = (char*)realloc(newToken, (len + 1) * sizeof(char));
                newToken[len] = *ptr++; }
            newToken[len] = '\0'; }
        break; }
        if(newToken != NULL) {
            numTokens++;
            tokens = (char**)realloc(tokens, numTokens * sizeof(char*));
            tokens[numTokens - 1] = newToken; } }
    *tokensRef = tokens;
    return numTokens; }
bool leftAssoc(token op) {
    bool ret;
    switch(tokenType(op)) {
    case addop:
    case multop:
        ret = true;
        break;
    case expop:
        ret = false;
        break; }
    return ret; }
int precedence(token op1, token op2) {
    int ret;
    if(tokenType(op1) == tokenType(op2))
        ret = 0;
    else if(tokenType(op1) == addop
            && (tokenType(op2) == multop || tokenType(op2) == expop))
        ret = -1;
    else if(tokenType(op2) == addop
            && (tokenType(op1) == multop || tokenType(op1) == expop))
        ret = 1;
    else if(tokenType(op1) == multop
            && tokenType(op2) == expop)
        ret = -1;
    else if(tokenType(op1) == expop
            && tokenType(op2) == multop)
        ret = 1;
    return ret; }
void evalStackPush(Stack *s, token val) {
    if(prefs.display.postfix)
        printf("\t%s\n", val);
    switch(tokenType(val)) {
    case function: {
        token operand, res;
        operand = (token)stackPop(s);
        res = doFunc(operand, val);
        stackPush(s, res); }
    break;
    case expop:
    case multop:
    case addop: {
        if(stackSize(s) >= 2) {
            token l, r, res;
            r = (token)stackPop(s);
            l = (token)stackPop(s);
            res = doOp(l, val, r);
            stackPush(s, res); }
        else {
            stackPush(s, val); } }
    break;
    case value: {
        stackPush(s, val); }
    break; } }
bool postfix(token *tokens, int numTokens, Stack *output) {
    Stack operators;
    int i;
    bool err = false;
    stackInit(&operators);
    for(i = 0; i < numTokens; i++) {
        switch(tokenType(tokens[i])) {
        case value: {
            evalStackPush(output, tokens[i]); }
        break;
        case function: {
            stackPush(&operators, tokens[i]); }
        break;
        case argsep: {
            while(stackSize(&operators) > 0
                    && tokenType((token)stackTop(&operators)) != lparen
                    && stackSize(&operators) > 1) {
                evalStackPush(output, stackPop(&operators)); }
            if(stackSize(&operators) > 0
                    && tokenType((token)stackTop(&operators)) != lparen) {
                err = true;
                raise(parenMismatch); }
            stackPop(&operators); }
        break;
        case addop:
        case multop:
        case expop: {
            while(stackSize(&operators) > 0
                    && (tokenType((char*)stackTop(&operators)) == addop || tokenType((char*)stackTop(&operators)) == multop || tokenType((char*)stackTop(&operators)) == expop)
                    && ((leftAssoc(tokens[i]) && precedence(tokens[i], (char*)stackTop(&operators)) <= 0)
                        || (!leftAssoc(tokens[i]) && precedence(tokens[i], (char*)stackTop(&operators)) < 0))) {
                evalStackPush(output, stackPop(&operators)); }
            stackPush(&operators, tokens[i]); }
        break;
        case lparen: {
            stackPush(&operators, tokens[i]); }
        break;
        case rparen: {
            while(stackSize(&operators) > 0
                    && tokenType((token)stackTop(&operators)) != lparen
                    && stackSize(&operators) > 1) {
                evalStackPush(output, stackPop(&operators)); }
            if(stackSize(&operators) > 0
                    && tokenType((token)stackTop(&operators)) != lparen) {
                err = true;
                raise(parenMismatch); }
            stackPop(&operators); }
        break; } }
    while(stackSize(&operators) > 0) {
        if(tokenType((token)stackTop(&operators)) == lparen) {
            raise(parenMismatch);
            err = true; }
        evalStackPush(output, stackPop(&operators)); }
    return err; }
int strSplit(char *str, const char split, char *(**partsRef)) {
    char **parts = NULL;
    char *ptr = str;
    char *part = NULL;
    int numParts = 0;
    char ch;
    int len = 0;
    while(1) {
        ch = *ptr++;
        if((ch == '\0' || ch == split) && part != NULL) {
            part = (char*)realloc(part, (len+1) * sizeof(char));
            part[len] = '\0';
            numParts++;
            if(parts == NULL)
                parts = (char**)malloc(sizeof(char**));
            else
                parts = (char**)realloc(parts, numParts * sizeof(char*));
            parts[numParts - 1] = part;
            part = NULL;
            len = 0; }
        else {
            len++;
            if(part == NULL)
                part = (char*)malloc(sizeof(char));
            else
                part = (char*)realloc(part, len * sizeof(char));
            part[len - 1] = ch; }
        if(ch == '\0')
            break; }
    *partsRef = parts;
    return numParts; }
bool execCommand(char *str) {
    bool recognized = false;
    char **words = NULL;
    int len = strSplit(str, ' ', &words);
    if(len >= 1 && strcmp(words[0], "get") == 0) {
        if(len >= 2 && strcmp(words[1], "display") == 0) {
            if(len >= 3 && strcmp(words[2], "tokens") == 0) {
                recognized = true;
                printf("\t%s\n", (prefs.display.tokens ? "on" : "off")); }
            else if(len >= 3 && strcmp(words[2], "postfix") == 0) {
                recognized = true;
                printf("\t%s\n", (prefs.display.postfix ? "on" : "off")); } }
        else if(len >= 2 && strcmp(words[1], "mode") == 0) {
            recognized = true;
            printf("\t%s\n", (prefs.mode.degrees ? "degrees" : "radians")); } }
    else if(len >= 1 && strcmp(words[0], "set") == 0) {
        if(len >= 2 && strcmp(words[1], "display") == 0) {
            if(len >= 3 && strcmp(words[2], "tokens") == 0) {
                if(len >= 4 && strcmp(words[3], "on") == 0) {
                    recognized = true;
                    prefs.display.tokens = true; }
                else if(len >= 4 && strcmp(words[3], "off") == 0) {
                    recognized = true;
                    prefs.display.tokens = false; } }
            else if(len >= 3 && strcmp(words[2], "postfix") == 0) {
                if(len >= 4 && strcmp(words[3], "on") == 0) {
                    recognized = true;
                    prefs.display.postfix = true; }
                else if(len >= 4 && strcmp(words[3], "off") == 0) {
                    recognized = true;
                    prefs.display.postfix = false; } } }
        else if(len >= 2 && strcmp(words[1], "mode") == 0) {
            if(len >= 3 && strcmp(words[2], "radians") == 0) {
                recognized = true;
                prefs.mode.degrees = false; }
            else if(len >= 3 && strcmp(words[2], "degrees") == 0) {
                recognized = true;
                prefs.mode.degrees = true; } } }
    return recognized; }
int main(int argc, char *argv[]) {
    char* str = NULL;
    token* tokens = NULL;
    int numTokens = 0;
    Stack expr;
    int i;
    int ch, rflag = 0;
    int count=0;
    while ((ch = getopt(argc, argv, "r")) != -1) {
        switch (ch) {
        case 'r':
            rflag = 1;
            break; } }
#ifdef REAL
    str = ufgets(stdin);
#else
    str = malloc(10);
    klee_make_symbolic(str,10,"str");
#endif
    while(str != NULL && strcmp(str, "quit") != 0) {
        if(type(*str) == text && execCommand(str)) {
            free(str);
            str = NULL; }
        else {
            numTokens = tokenize(str, &tokens);
            free(str);
            str = NULL;
            if(prefs.display.tokens) {
                printf("\t%d tokens:\n", numTokens);
                for(i = 0; i < numTokens; i++) {
                    printf("\t\"%s\"", tokens[i]);
                    if(tokenType(tokens[i]) == value)
                        printf(" = %d", buildNumber(tokens[i]));
                    printf("\n"); } }
            stackInit(&expr);
            if(prefs.display.postfix)
                printf("\tPostfix stack:\n");
            postfix(tokens, numTokens, &expr);
            if(stackSize(&expr) != 1) {
                printf("\tError evaluating expression\n"); }
            else {
                if (!rflag)
                    printf("\t= ");
                printf("%s\n", (char*)stackTop(&expr)); }
            for(i = 0; i < numTokens; i++) {
                free(tokens[i]); }
            free(tokens);
            tokens = NULL;
            numTokens = 0;
            stackFree(&expr); }

        count++;
        if (count>=1) break;
#ifdef REAL
        str = ufgets(stdin);
#else
        str = malloc(10);
        strcpy(str,"4*5");
#endif
    }
    free(str);
    str = NULL;
    return EXIT_SUCCESS; }
