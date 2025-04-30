Coding Standards:

Functions: 
UpperCamelCase () 
{

}

Globals constants:
FULL_UPPER_CASE

variable name:
camelCase 

static/singleton:
static int sInstance;

class PascalCase
{
public:
    int  variableName;
    void FunctionName ();

private:
    int* m_pName;
    bool m_isBool;

    void OtherFunctionName ();
};

bool Functions:
bool isSomething () {}
if (isSomething())
{

}

File Header:

// Primary Contributer: Name
// Additional Contributers
//      Name
//      Name
