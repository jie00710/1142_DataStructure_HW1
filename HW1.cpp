#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include <sstream>
using namespace std;
// 定義 Stack 的節點結構 (存儲字符)
struct Node {
char data; // 存儲字符 (運算子或括號)
Node* next; // 指向下一個節點
};
// 使用 linked list 實作 Stack
class Stack {
private:
Node* top; // 指向堆疊頂端
public:
Stack() { top = nullptr; } // 初始化堆疊
// Push 操作:將元素放入堆疊
void push(char ch) {
Node* newNode = new Node; // 創建新節點
newNode->data = ch; // 存入數據
newNode->next = top; // 讓新節點指向目前的頂端
top = newNode; // 更新頂端指標
}
// Pop 操作:移除並回傳頂端元素
char pop() {
if (isEmpty()) return '\0'; // 若堆疊為空,回傳空字符
char ch = top->data; // 取得頂端元素
Node* temp = top; // 暫存頂端節點

top = top->next; // 移動頂端指標
delete temp; // 釋放記憶體
return ch; // 回傳彈出的元素
}
// Peek 操作:取得頂端元素但不移除
char peek() {
return (top ? top->data : '\0');
}
// 判斷堆疊是否為空
bool isEmpty() {
return top == nullptr;
}
};

// 定義存儲浮點數的 Stack 節點結構
struct NumNode {
    double data; // 存儲數字
    NumNode* next; // 指向下一個節點
};

// 使用 linked list 實作浮點數 Stack
class NumberStack {
private:
    NumNode* top; // 指向堆疊頂端
public:
    NumberStack() { top = nullptr; } // 初始化堆疊
    
    void push(double num) {
        NumNode* newNode = new NumNode;
        newNode->data = num;
        newNode->next = top;
        top = newNode;
    }
    
    double pop() {
        if (isEmpty()) return 0.0;
        double num = top->data;
        NumNode* temp = top;
        top = top->next;
        delete temp;
        return num;
    }
    
    double peek() {
        return (top ? top->data : 0.0);
    }
    
    bool isEmpty() {
        return top == nullptr;
    }
    
    ~NumberStack() {
        while (!isEmpty()) pop();
    }
};
bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || 
            ch == '%' || ch == '^');
}

// 判斷運算子的優先順序
int precedence(char op) {
if (op == '^') {
    return 10;
} 
else if (op == '*' || op == '/' || op == '%') {
    return 9; 
}
else if (op == '+' || op == '-') {
    return 8; 
}
else if (op == '<' || op == '>') {
    return 7; 
}
else if (op == '=' || op == '!') {
    return 6; 
}
/*else if (op == "~" ) {
    return 5; 
}
else if (op == "&" ) {
    return 4; 
}
else if (op == "^") {
    return 3; 
}
else if (op == "|" || op == "-") {
    return 2; 
}
else if (op == "<<" || op == ">>") {
    return 1; 
}*/
else{
    return -1; // 其他字符的優先順序為 -1
}
}

// 計算兩個數字的運算結果
double calculate(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                cout << "Error: Division by zero!" << endl;
                return 0;
            }
            return a / b;
        case '%': return (int)a % (int)b;
        case '^': {
            double result = 1;
            for (int i = 0; i < (int)b; i++) result *= a;
            return result;
        }
        default: return 0;
    }
}

// 評估後序表達式並計算結果 - 帶詳細調試信息
double evaluatePostfix(const string& postfix) {
    NumberStack s;
    stringstream ss(postfix);
    string token;
    int step = 1;
    
    cout << "\n--- Evaluation Steps ---" << endl;
    
    while (ss >> token) {
        // 判斷是否為運算子
        if (token.length() == 1 && isOperator(token[0])) {
            // 需要至少兩個運算元
            if (s.isEmpty()) {
                cout << "Error: Invalid postfix expression (insufficient operands)!" << endl;
                return 0;
            }
            
            double b = s.pop();
            if (s.isEmpty()) {
                cout << "Error: Invalid postfix expression (insufficient operands)!" << endl;
                return 0;
            }
            
            double a = s.pop();
            double result = calculate(a, b, token[0]);
            cout << "Step " << step++ << ": " << a << " " << token[0] << " " << b 
                 << " = " << result << endl;
            s.push(result);
        }
        else {
            // 是運算元，轉換為浮點數
            try {
                double num = stod(token);
                cout << "Step " << step++ << ": Push " << num << endl;
                s.push(num);
            }
            catch (...) {
                cout << "Error: Invalid token '" << token << "'!" << endl;
                return 0;
            }
        }
    }
    
    // 最後 stack 中應該只有一個元素
    if (s.isEmpty()) {
        cout << "Error: Invalid postfix expression (empty)!" << endl;
        return 0;
    }
    
    double result = s.pop();
    if (!s.isEmpty()) {
        cout << "Error: Invalid postfix expression (too many operands)!" << endl;
        return 0;
    }
    
    cout << "\n";
    return result;
}

// 將中序表達式 (infix) 轉換為後序表達式 (postfix)
// 運算元使用空格分隔
void InfixToPostfix(const string& infix, string& postfix) {
    Stack s; // 創建堆疊
    postfix = ""; // 初始化後序表達式
    int i = 0;
    int parenCount = 0; // 追蹤括號配對
    
    while (i < infix.length()) {
        // 跳過空格
        while (i < infix.length() && infix[i] == ' ') {
            i++;
        }
        
        if (i >= infix.length()) break;
        
        char ch = infix[i];
        
        // 如果是數字或小數點，讀取完整的數字
        if (isdigit(ch) || ch == '.') {
            if (!postfix.empty() && postfix.back() != ' ') {
                postfix += ' '; // 添加空格分隔
            }
            while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.')) {
                postfix += infix[i];
                i++;
            }
            postfix += ' '; // 運算元後添加空格
        }
        else if (ch == '(') { // 左括號 push 進 stack
            s.push(ch);
            parenCount++; // 遞增括號計數
            i++;
        } 
        else if (ch == ')') { // 遇到右括號,pop 出 stack 中的元素直到左括號
            if (parenCount == 0) {
                cout << "Error: Unmatched right parenthesis at position " << i << "!" << endl;
                postfix = "";
                return;
            }
            
            while (!s.isEmpty() && s.peek() != '(') {
                if (!postfix.empty() && postfix.back() != ' ') {
                    postfix += ' ';
                }
                postfix += s.pop();
                postfix += ' ';
            }
            
            if (s.isEmpty()) {
                cout << "Error: Unmatched right parenthesis at position " << i << "!" << endl;
                postfix = "";
                return;
            }
            
            s.pop(); // pop 出左括號
            parenCount--; // 遞減括號計數
            i++;
        } 
        else if (isOperator(ch)) { // 遇到 operator
            while (!s.isEmpty() && s.peek() != '(' && isOperator(s.peek()) && 
                   precedence(s.peek()) >= precedence(ch)) {
                if (!postfix.empty() && postfix.back() != ' ') {
                    postfix += ' ';
                }
                postfix += s.pop();
                postfix += ' ';
            }
            s.push(ch); // 將當前讀取的元素 push 進 stack
            i++;
        }
        else {
            cout << "Error: Invalid character '" << ch << "' at position " << i << "!" << endl;
            postfix = "";
            return;
        }
    }
    
    // 檢查是否有未配對的左括號
    if (parenCount != 0) {
        cout << "Error: Unmatched left parenthesis!" << endl;
        postfix = "";
        return;
    }
    
    // 輸出堆疊中剩餘的元素
    while (!s.isEmpty()) {
        if (!postfix.empty() && postfix.back() != ' ') {
            postfix += ' ';
        }
        postfix += s.pop();
        postfix += ' ';
    }
    
    // 去除末尾空格
    while (!postfix.empty() && postfix.back() == ' ') {
        postfix.pop_back();
    }
}
int main() {
    string infix, postfix;
    cout << "Enter an Infix expression (e.g., 12.5 + 3.14 * 2): ";
    getline(cin, infix); // 輸入中序表達式
    InfixToPostfix(infix, postfix); // 轉換為後序表達式
    cout << "Postfix expression: " << postfix << endl; // 輸出後序表達式
    
    double result = evaluatePostfix(postfix); // 計算後序表達式的結果
    cout << "Result: " << result << endl; // 輸出結果
    
    return 0;
}
