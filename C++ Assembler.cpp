#include<bits/stdc++.h>
using namespace std;

map<string,string> Literals;
map<string,string> Symbols;
map<string,string> MOT;
string IC="";
map<string,string>::iterator itr;
map<string,string>::reverse_iterator itr1;
vector<int> LC;

bool isnotdigit(const char in)
{
    return (in<'0'||in>'9');
}
bool is_numeric(string in)
{
    string::const_iterator it = in.begin();
    while (it!=in.end()&&isdigit(*it))
        ++it;
    return !in.empty()&&it==in.end();
}
bool expr(char in)
{
    return (in=='+'||in=='-'||in==','||in=='/'||in=='*');
}

int Find_Symbol_Position(string in)
{
    int i=0;
    for(itr1=Symbols.rbegin(); itr1!=Symbols.rend(); itr1++,i++)
    {
        if(in==itr1->first)
            return i;
    }
    return -1;
}

int Find_Literal_Position(string in)
{
    int i=0;
    for(itr1=Literals.rbegin(); itr1!=Literals.rend(); itr1++,i++)
    {
        if(in==itr1->first)
            return i;
    }
    return -1;
}

map<string,string>::iterator find_unassigned_literal(int &in)
{
    in=0;
    for(itr=Literals.begin(); itr!=Literals.end(); itr++,in++)
    {
        if(itr->second == "-")
        {
            return itr;
        }
    }
    return itr;
}
string int_to_string(int in)
{
    stringstream temp;
    temp<<in;
    string res;
    temp>>res;
    return res;
}
int string_to_int(string in)
{
    int res=0;
    for(int i=0; i<in.length(); i++)
    {
        res=res*10+(int)(in.at(i)-'0');
    }
    return res;
}

void fillMOT()
{
    MOT.insert(pair<string,string>("STOP","IS,00"));
    MOT.insert(pair<string,string>("ADD","IS,01"));
    MOT.insert(pair<string,string>("SUB","IS,02"));
    MOT.insert(pair<string,string>("MULTI","IS,03"));
    MOT.insert(pair<string,string>("MOVER","IS,04"));
    MOT.insert(pair<string,string>("MOVEM","IS,05"));
    MOT.insert(pair<string,string>("COMB","IS,06"));
    MOT.insert(pair<string,string>("BC","IS,07"));
    MOT.insert(pair<string,string>("DIV","IS,08"));
    MOT.insert(pair<string,string>("READ","IS,09"));
    MOT.insert(pair<string,string>("PRINT","IS,10"));
    MOT.insert(pair<string,string>("END","AD,02"));
    MOT.insert(pair<string,string>("START","AD,01"));
    MOT.insert(pair<string,string>("ORIGIN","AD,03"));
    MOT.insert(pair<string,string>("EQU","AD,04"));
    MOT.insert(pair<string,string>("LTORG","AD,05"));
    MOT.insert(pair<string,string>("DS","DL,01"));
    MOT.insert(pair<string,string>("DC","DL,02"));
    MOT.insert(pair<string,string>("AREG","RG,01"));
    MOT.insert(pair<string,string>("BREG","RG,02"));
    MOT.insert(pair<string,string>("CREG","RG,03"));
    MOT.insert(pair<string,string>("EQ","CC,01"));
    MOT.insert(pair<string,string>("LT","CC,02"));
    MOT.insert(pair<string,string>("GT","CC,03"));
    MOT.insert(pair<string,string>("LE","CC,04"));
    MOT.insert(pair<string,string>("GE","CC,05"));
    MOT.insert(pair<string,string>("NE","CC,06"));
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie();
    fillMOT();
    fstream fin;
    fin.open("test.txt",ios::in);
    string line;
    string term;

    int counter=0;

    while(getline(fin,line))
    {
        LC.push_back(counter);
        stringstream ss(line);
        if(ss>>term)
        {
            if(MOT.count(term)!=0)
            {

                if(term=="START")
                {
                    IC+=("("+MOT.find(term)->second+") ");

                    if(ss>>term)
                    {
                        counter=string_to_int(term);
                        IC+=("(C,"+term+") ");
                        IC+="\n";
                    }
                    else
                    {
                        cout<<"ERROR: No starting location specified at "<<counter<<"!\n";
                    }
                }
                else if(term=="LTORG")
                {
                    int x;
                    int tempnum=0;
                    while((itr=find_unassigned_literal(x))!=Literals.end())
                    {
                        if(tempnum!=0)
                            LC.push_back(counter);
                        string temp=itr->first;
                        temp.erase(remove_if(temp.begin(),temp.end(),isnotdigit),temp.end());
                        IC+=("("+MOT.find(term)->second+") "+"(DL,02) "+"(C,"+temp+") \n");
                        itr->second=int_to_string(counter);
                        counter++;
                        tempnum++;
                    }
                }
                else if(term=="END")
                {
                    int x;
                    if((itr=find_unassigned_literal(x))!=Literals.end())
                    {
                        string tempstr = itr->first;
                        tempstr.erase(remove_if(tempstr.begin(),tempstr.end(),isnotdigit),tempstr.end());
                        IC+=("("+MOT.find(term)->second+") "+"(DL,02) "+"(C,"+tempstr+") \n");
                        itr->second=int_to_string(counter);
                        counter++;
                        while((itr=find_unassigned_literal(x))!=Literals.end())
                        {
                            LC.push_back(counter);
                            tempstr = itr->first;
                            tempstr.erase(remove_if(tempstr.begin(),tempstr.end(),isnotdigit),tempstr.end());
                            IC+=("("+MOT.find(term)->second+") "+"(DL,02) "+"(C,"+tempstr+") \n");
                            itr->second=int_to_string(counter);
                            counter++;
                        }
                        LC.push_back(counter);
                        IC+=("("+MOT.find(term)->second+") ");
                    }
                    else
                        IC+=("("+MOT.find(term)->second+") ");
                }
                else if(term=="ORIGIN")
                {
                    int counttemp=0;
                    IC+=("("+MOT.find(term)->second+") ");
                    if(ss.rdbuf()->in_avail()==0)
                    {
                        cout<<"ERROR: No operands specified at "<<counter<<"!\n";
                    }
                    while(ss>>term)
                    {
                        replace_if(term.begin(),term.end(),expr,' ');
                        stringstream ss1(term);
                        while(ss1>>term)
                        {
                            if(MOT.count(term)!=0)
                            {
                                IC+=("("+MOT.find(term)->second+") ");
                            }
                            else
                            {
                                if(is_numeric(term))
                                {
                                    counttemp+=string_to_int(term);
                                }
                                else
                                {
                                    if(Symbols.count(term)!=0&&Symbols.find(term)->second!="-")
                                    {
                                        counttemp+=string_to_int(Symbols.find(term)->second);
                                    }
                                    else
                                    {
                                        cout<<"ERROR: Value of Symbol is undefined at "<<counter<<"!\n";
                                    }
                                }
                            }
                        }
                    }
                    IC+=("(C,"+int_to_string(counttemp)+") \n");
                    counter=counttemp;
                }
                else
                {
                    IC+=("("+MOT.find(term)->second+") ");
                    if(ss.rdbuf()->in_avail()==0)
                    {
                        cout<<"ERROR: No operands specified at "<<counter<<"!\n";
                    }
                    while(ss>>term)
                    {
                        int pos;
                        if((pos = term.find('='))!=string::npos)
                        {
                            string temp1=term.substr(pos);
                            term.erase(term.begin()+pos,term.end());

                            if(term!="")
                            {
                                if(MOT.count(term)!=0)
                                {
                                    IC+=("("+MOT.find(term)->second+") ");
                                }
                                else
                                {
                                    if(is_numeric(term))
                                    {
                                        cout<<"ERROR: Symbol expected at "<<counter<<"!\n";
                                    }
                                    else
                                    {
                                        if(Symbols.count(term)!=0)
                                        {
                                            IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                        }
                                        else
                                        {
                                            Symbols.insert(pair<string,string>(term,"-"));
                                            IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                        }
                                    }
                                }
                            }

                            if(temp1.at(1)=='\'' && temp1.at(temp1.length()-1)=='\'')
                            {
                                if(Literals.count(temp1)!=0)
                                {
                                    IC+=("(L,"+int_to_string(Find_Literal_Position(temp1))+") ");
                                }
                                else
                                {
                                    Literals.insert(pair<string,string>(temp1,"-"));
                                    IC+=("(L,"+int_to_string(Find_Literal_Position(temp1))+") ");
                                }
                            }
                        }
                        else
                        {
                            replace_if(term.begin(),term.end(),expr,' ');
                            stringstream ss1(term);
                            while(ss1>>term)
                            {
                                if(MOT.count(term)!=0)
                                {
                                    IC+=("("+MOT.find(term)->second+") ");
                                }
                                else
                                {
                                    if(is_numeric(term))
                                    {
                                        IC+=("(C,"+term+") ");
                                    }
                                    else
                                    {
                                        if(Symbols.count(term)!=0)
                                        {
                                            IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                        }
                                        else
                                        {
                                            Symbols.insert(pair<string,string>(term,"-"));
                                            IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                        }
                                    }
                                }
                            }
                        }
                    }
                    IC+="\n";
                    counter++;
                }

            }





            else
            {
                int pos;
                if(Symbols.count(term)!=0)
                    Symbols.find(term)->second=int_to_string(counter);
                else
                    Symbols.insert(pair<string,string>(term,int_to_string(counter)));

                IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");

                if(ss>>term)
                {
                    if(MOT.count(term)!=0)
                    {
                        if(term=="START")
                        {
                            IC+=("("+MOT.find(term)->second+") ");

                            if(ss>>term)
                            {
                                counter=string_to_int(term);
                                IC+=("(C,"+term+") ");
                                IC+="\n";
                            }
                            else
                            {
                                cout<<"ERROR: No starting location specified at "<<counter<<"!\n";
                            }
                        }
                        else if(term=="LTORG")
                        {
                            int x;
                            int tempnum=0;
                            while((itr=find_unassigned_literal(x))!=Literals.end())
                            {
                                if(tempnum!=0)
                                    LC.push_back(counter);
                                string temp=itr->first;
                                temp.erase(remove_if(temp.begin(),temp.end(),isnotdigit),temp.end());
                                IC+=("("+MOT.find(term)->second+") "+"(DL,02) "+"(C,"+temp+") \n");
                                itr->second=int_to_string(counter);
                                counter++;
                                tempnum++;
                            }
                        }
                        else if(term=="END")
                        {
                            int x;
                            if((itr=find_unassigned_literal(x))!=Literals.end())
                            {
                                string tempstr = itr->first;
                                tempstr.erase(remove_if(tempstr.begin(),tempstr.end(),isnotdigit),tempstr.end());
                                IC+=("("+MOT.find(term)->second+") "+"(DL,02) "+"(C,"+tempstr+") \n");
                                itr->second=int_to_string(counter);
                                counter++;
                                while((itr=find_unassigned_literal(x))!=Literals.end())
                                {
                                    LC.push_back(counter);
                                    tempstr = itr->first;
                                    tempstr.erase(remove_if(tempstr.begin(),tempstr.end(),isnotdigit),tempstr.end());
                                    IC+=("("+MOT.find(term)->second+") "+"(DL,02) "+"(C,"+tempstr+") \n");
                                    itr->second=int_to_string(counter);
                                    counter++;
                                }
                                LC.push_back(counter);
                                IC+=("("+MOT.find(term)->second+") ");
                            }
                            else
                                IC+=("("+MOT.find(term)->second+") ");
                        }
                        else if(term=="ORIGIN")
                        {
                            int counttemp=0;
                            IC+=("("+MOT.find(term)->second+") ");
                            if(ss.rdbuf()->in_avail()==0)
                            {
                                cout<<"ERROR: No operands specified at "<<counter<<"!\n";
                            }
                            while(ss>>term)
                            {
                                replace_if(term.begin(),term.end(),expr,' ');
                                stringstream ss1(term);
                                while(ss1>>term)
                                {
                                    if(MOT.count(term)!=0)
                                    {
                                        IC+=("("+MOT.find(term)->second+") ");
                                    }
                                    else
                                    {
                                        if(is_numeric(term))
                                        {
                                            counttemp+=string_to_int(term);
                                        }
                                        else
                                        {
                                            if(Symbols.count(term)!=0&&Symbols.find(term)->second!="-")
                                            {
                                                counttemp+=string_to_int(Symbols.find(term)->second);
                                            }
                                            else
                                            {
                                                cout<<"ERROR: Value of Symbol is undefined at "<<counter<<"!\n";
                                            }
                                        }
                                    }
                                }
                            }
                            IC+=("(C,"+int_to_string(counttemp)+") \n");
                            counter=counttemp;
                        }
                        else
                        {
                            IC+=("("+MOT.find(term)->second+") ");
                            if(ss.rdbuf()->in_avail()==0)
                            {
                                cout<<"ERROR: No operands specified at "<<counter<<"!\n";
                            }
                            while(ss>>term)
                            {
                                int pos;
                                if((pos = term.find('='))!=string::npos)
                                {
                                    string temp1=term.substr(pos);
                                    term.erase(term.begin()+pos,term.end());

                                    if(term!="")
                                    {
                                        if(MOT.count(term)!=0)
                                        {
                                            IC+=("("+MOT.find(term)->second+") ");
                                        }
                                        else
                                        {
                                            if(is_numeric(term))
                                            {
                                                cout<<"ERROR: Symbol expected at "<<counter<<"!\n";
                                            }
                                            else
                                            {
                                                if(Symbols.count(term)!=0)
                                                {
                                                    IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                                }
                                                else
                                                {
                                                    Symbols.insert(pair<string,string>(term,"-"));
                                                    IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                                }
                                            }
                                        }
                                    }

                                    if(temp1.at(1)=='\'' && temp1.at(temp1.length()-1)=='\'')
                                    {
                                        if(Literals.count(temp1)!=0)
                                        {
                                            IC+=("(L,"+int_to_string(Find_Literal_Position(temp1))+") ");
                                        }
                                        else
                                        {
                                            Literals.insert(pair<string,string>(temp1,"-"));
                                            IC+=("(L,"+int_to_string(Find_Literal_Position(temp1))+") ");
                                        }
                                    }
                                }
                                else
                                {
                                    replace_if(term.begin(),term.end(),expr,' ');
                                    stringstream ss1(term);
                                    while(ss1>>term)
                                    {
                                        if(MOT.count(term)!=0)
                                        {
                                            IC+=("("+MOT.find(term)->second+") ");
                                        }
                                        else
                                        {
                                            if(is_numeric(term))
                                            {
                                                IC+=("(C,"+term+") ");
                                            }
                                            else
                                            {
                                                if(Symbols.count(term)!=0)
                                                {
                                                    IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                                }
                                                else
                                                {
                                                    Symbols.insert(pair<string,string>(term,"-"));
                                                    IC+=("(S,"+int_to_string(Find_Symbol_Position(term))+") ");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            IC+="\n";
                            counter++;
                        }
                    }
                    else
                    {
                        cout<<"ERROR: Assembler operation missing at "<<counter<<"!\n";
                        counter++;
                        IC+="\n";
                    }
                }
            }
        }
        else
            cout<<"ERROR: Empty line in file!";
    }

    cout<<"\n\nLocation counter:\n";
    for(vector<int>::iterator it=LC.begin(); it!=LC.end(); it++)
    {
        cout<<*it<<"\n";
    }

    cout<<"\n\nIntermediate code:\n"<<IC<<"\n\nSymbol table:\n";
    for(itr=Symbols.begin(); itr!=Symbols.end(); itr++)
    {
        cout<<itr->first<<"\t"<<itr->second<<"\n";
    }
    cout<<"\n\nLiteral table:\n";
    for(itr=Literals.begin(); itr!=Literals.end(); itr++)
    {
        cout<<itr->first<<"\t"<<itr->second<<"\n";
    }
    return 0;
}
