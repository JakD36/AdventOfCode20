#include <algorithm>
#include <chrono>
#include <vector>
#include <set>

enum class Type
{
    nop,
    acc,
    jmp
};

struct Instruction
{
public:
    Type m_type;
    int m_val;
};

void Part1(FILE* file);
void Part2(FILE* file);
void Part2Brute(FILE* file);
void Analyse(FILE* file);

std::vector<Instruction> Parse(FILE* file);
void ExecuteBootCode(std::vector<Instruction> &instructions);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Analyse(file);
    }
    else
    {
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Part1(FILE* file)
{
    std::set<int> visitedInstructions;

    auto instructions = Parse(file);
    int global = 0, i = 0;
    while(true)
    {
        if (visitedInstructions.emplace(i).second == false)
        {
            printf("global %d\n",global);
            break;
        }
        switch(instructions[i].m_type)
        {
            case Type::jmp:
                i += instructions[i].m_val;
                break;
            case Type::acc:
                global += instructions[i].m_val;
                ++i;
                break;
            case Type::nop:
                ++i;
                break;
        }
    }
}

void Part2(FILE* file)
{
    auto instructions = Parse(file);
    int instructionCount = instructions.size();
    int* executionPath = new int[instructionCount];
    std::fill(executionPath,executionPath+instructionCount,-1);


    int global = 0, i = 0, n = -1;
    bool fixed = false;
    int executionCount = 0;
    while(i < instructionCount && executionCount < instructionCount)
    {
        auto find = std::find(executionPath,executionPath + instructionCount, i);
        if (find == (executionPath + instructionCount))
        {
            executionPath[++n] = i;
        }
        else
        {
            for(int j = 0; j < n; ++j)
            {
                printf("%d ",executionPath[j]);
            }
            printf("\n");
            break;
        }

        switch(instructions[i].m_type)
        {
            case Type::jmp:
                i += instructions[i].m_val;
                break;
            case Type::acc:
                global += instructions[i].m_val;
                ++i;
                break;
            case Type::nop:
                ++i;
                break;
        }
        ++executionCount;
    }
    if(executionCount == instructionCount)
    {
        printf("Error stuck in an infinite loop!\n");
        std::sort(executionPath,executionPath+executionCount);
        for(int j = 0; j < executionCount; ++j)
        {
            printf("%d ",executionPath[j]);
        }
        printf("\n");
    }
    else
    {
        printf("%d\n",global);
    }

}

void Part2Brute(FILE* file) // Instruction 363 is the problem (line 364)
{
    auto instructions = Parse(file);
    int instructionCount = instructions.size();

    for(int n = 0; n < instructionCount; ++n)
    {
        int global = 0, i = 0;
        int executionCount = 0;
        if(instructions[n].m_type == Type::acc)
            continue;

        instructions[n].m_type = instructions[n].m_type == Type::nop ? Type::jmp : Type::nop;

        while(i < instructionCount && executionCount < instructionCount)
        {
            switch(instructions[i].m_type)
            {
                case Type::jmp:
                    i += instructions[i].m_val;
                    break;
                case Type::acc:
                    global += instructions[i].m_val;
                    ++i;
                    break;
                case Type::nop:
                    ++i;
                    break;
            }
            ++executionCount;
        }
        if(executionCount == instructionCount)
        {
            printf("Error %d\n",n);
            instructions[n].m_type = instructions[n].m_type == Type::nop ? Type::jmp : Type::nop;
        }
        else
        {
            printf("%d\n",global);
            break;
        }
    }


}

void Analyse()
{
    
}


std::vector<Instruction> Parse(FILE* file)
{
    std::vector<Instruction> bootCode;
    bootCode.reserve(100);
    char opStr[4];
    int val;
    while(fscanf(file,"%s %d",opStr,&val) != EOF)
    {
        if(strcmp(opStr,"nop") == 0)
        {
            bootCode.push_back(Instruction{Type::nop, val});
        }
        else if(strcmp(opStr,"acc") == 0)
        {
            bootCode.push_back(Instruction{Type::acc, val});
        }
        else if(strcmp(opStr,"jmp") == 0)
        {
            bootCode.push_back(Instruction{Type::jmp, val});
        } else
            assert("Error: Unexpected Instruction type!");
    }
    return bootCode;
}

void ExecuteBootCode(std::vector<Instruction> &instructions)
{
    int global = 0, i = 0;
    int instructionCount = instructions.size();
    while(i < instructionCount)
    {
        switch(instructions[i].m_type)
        {
            case Type::jmp:
                i += instructions[i].m_val;
                break;
            case Type::acc:
                global += instructions[i].m_val;
                ++i;
                break;
            case Type::nop:
                ++i;
                break;
            default:
                assert("Unexpected instruction."); break;
        }
    }
    printf("Global %d\n",global);
}