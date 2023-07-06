#include "transform.hpp"

#include <cstring>
#include <sstream>

int32_t convert_string_to_long(const std::string &str){
  return atoi(str.c_str());
}

uint32_t convert_string_to_ulong(const std::string &str){
  return strtoul(str.c_str(),NULL,0);
}

uint64_t convert_string_to_ulong64(const std::string &str){
  return strtoull(str.c_str(),NULL,0);
}

void add_long_to_string(int32_t n, std::string &str){
  std::string str2;
  convert_long_to_string(n,str2);
  str+=str2;
}

void add_ulong64_to_string(uint64_t n, std::string &str){
  std::string str2;
  convert_ulong64_to_string(n,str2);
  str+=str2;
}

void convert_long_to_string(int32_t n, std::string &str){
  std::ostringstream o;
  o << n;
  str = o.str();
}

void convert_ulong_to_string(uint32_t n, std::string &str){
  std::ostringstream o;
  o << n;
  str = o.str();
}

void convert_ulong64_to_string(uint64_t n, std::string &str){
  std::ostringstream o;
  o << n;
  str = o.str();
}

char grab_variable_token(std::string &str, std::string &token,char sec_comp,int32_t &pos,int32_t i_pos){

  token.erase();

  for(pos=i_pos;pos<str.length();pos++){
    if((str[pos]=='['&&pos+1<str.length())||sec_comp){
      if(str[pos]=='['&&!sec_comp)pos++;
      grab_token_string_pos(token,str,pos,':');
      pos--;

      if(token.length()>0)return 1;
    }
  }

  return 0;
}

bool grab_token_expression(std::string &dest,std::string &source,int32_t &pos,char compc){

  dest.erase();
  dest+="[";

  std::string token1;
  while(grab_token_string(token1,source,pos)){

    if(dest.length()>1)dest+=":";
    dest+=token1;

    if(pos<source.length()){
      if(source[pos]==']')break;//grab_token_string CAN'T HANDLE THESE
    }

  }
  dest+="]";

  return (dest.length()>2);
}

bool grab_token_list_as_string(std::string &dest,std::string &source,int32_t &pos,char compc){

  dest.erase();

  std::string token1;
  while(grab_token_string(token1,source,pos)){

    if(dest.length()>0)dest+=":";
    dest+=token1;

    if(pos<source.length()){
      if(source[pos]==']')break;//grab_token_string CAN'T HANDLE THESE
    }
  }

  return (dest.length()>0);
}

bool grab_token_string(std::string &dest,std::string &source,int32_t &pos,char compc){

  dest.erase();
  if(source.length()==0)return false;

  pos++;//GET RID OF FIRST [ OR compc THAT IS ASSUMED TO BE THERE
  if(pos>source.size())return false;

  //GO UNTIL YOU HIT A compc, ], or the end
  auto s=source.begin(),e=source.end();
  s+=pos;
  for(;s<e;++s){

    if((*s)==compc||(*s)==']')break;
    dest+=(*s);
    pos++;
  }
  return (dest.length()>0);
}

bool grab_token_string(std::string &dest,std::string &source,char compc){

  dest.erase();
  if(source.length()==0)return false;

  //GO UNTIL YOU HIT A :, ], or the end
  auto s=source.begin(),e=source.end();
  for(;s<e;++s)
  {
    if((*s)==compc||(*s)==']')break;
    dest+=(*s);
  }
  return (dest.length()>0);
}

bool grab_token_string_pos(std::string &dest,std::string &source,int32_t pos,char compc){

  dest.erase();
  if(source.length()==0)return false;
  if(pos>source.length())return false;

  //GO UNTIL YOU HIT A :, ], or the end
  auto s=source.begin(),e=source.end();
  s+=pos;
  for(;s<e;++s){
    if((*s)==compc||(*s)==']')
      break;
    dest+=(*s);
  }
  return (dest.length()>0);
}

bool grab_token_string(std::string &dest,const char *source,char compc){

  dest.erase();
  int32_t sz=(int32_t)strlen(source);
  if(sz==0)return false;

  //GO UNTIL YOU HIT A :, ], or the end
  int32_t s;
  for(s=0;s<sz;s++){
    if(source[s]==compc||source[s]==']')
      break;
    dest+=source[s];
  }
  return (dest.length()>0);
}


void replace_token_string(std::string &token,std::string &str,int32_t pos,char compc,std::string &nw,char repc){

  std::string rep;
  if(repc!=0)rep=repc;
  rep+=token;
  if(compc!=0)rep+=compc;

  std::string::size_type wpos;

  if ((wpos = str.find(rep)) != std::string::npos){
    str.replace(wpos,rep.size(),nw);
  }
}

void simplify_string(std::string &str){

  int32_t s;
  for(s=0;s<str.length();s++){
    //CAPITALIZE
    if(str[s]>='A'&&str[s]<='Z'){
      str[s]-='A';
      str[s]+='a';
    }

    switch(str[s]){
    case (char)129:
    case (char)150:
    case (char)151:
    case (char)154:
    case (char)163:
      str[s]='u';
      break;
    case (char)152:
      str[s]='y';
      break;
    case (char)164:
    case (char)165:
      str[s]='n';
      break;
    case (char)131:
    case (char)132:
    case (char)133:
    case (char)134:
    case (char)142:
    case (char)143:
    case (char)145:
    case (char)146:
    case (char)160:
      str[s]='a';
      break;
    case (char)130:
    case (char)136:
    case (char)137:
    case (char)138:
    case (char)144:
      str[s]='e';
      break;
    case (char)139:
    case (char)140:
    case (char)141:
    case (char)161:
      str[s]='i';
      break;
    case (char)147:
    case (char)148:
    case (char)149:
    case (char)153:
    case (char)162:
      str[s]='o';
      break;
    case (char)128:
    case (char)135:
      str[s]='c';
      break;
    }
  }
}

void lower_case_string(std::string &str){

  int32_t s;
  for(s=0;s<str.length();s++){
    //CAPITALIZE
    if(str[s]>='A'&&str[s]<='Z')
    {
      str[s]-='A';
      str[s]+='a';
    }
    switch(str[s])
    {
    case (char)154:str[s]=(char)129;break;
    case (char)165:str[s]=(char)164;break;
    case (char)142:str[s]=(char)132;break;
    case (char)143:str[s]=(char)134;break;
    case (char)144:str[s]=(char)130;break;
    case (char)153:str[s]=(char)148;break;
    case (char)128:str[s]=(char)135;break;
    case (char)146:str[s]=(char)145;break;
    }
  }
}

void upper_case_string(std::string &str){

  int32_t s;
  for(s=0;s<str.length();s++){

    //CAPITALIZE
    if(str[s]>='a'&&str[s]<='z'){
      str[s]-='a';
      str[s]+='A';
    }

    switch(str[s]){
      case (char)129:str[s]=(char)154;break;
      case (char)164:str[s]=(char)165;break;
      case (char)132:str[s]=(char)142;break;
      case (char)134:str[s]=(char)143;break;
      case (char)130:str[s]=(char)144;break;
      case (char)148:str[s]=(char)153;break;
      case (char)135:str[s]=(char)128;break;
      case (char)145:str[s]=(char)146;break;
    }
  }
}

void capitalize_string_words(std::string &str){

  bool starting=true;
  int32_t bracket_count=0;
  char conf;
  int32_t s;
  for(s=0;s<str.length();s++){

    if(str[s]=='['){++bracket_count;continue;}
    if(str[s]==']'){--bracket_count;continue;}
    if(bracket_count>0)continue;
    conf=0;
    if(!starting)
    {
      if(str[s-1]==' '||
        str[s-1]=='\"')conf=1;
        if(str[s-1]=='\'')
        {
        //DISCOUNT SINGLE QUOTE IF IT ISN'T PRECEDED BY SPACE, COMMA OR NOTHING
          if(s<=0)conf=1;
          else if(s>=2)
          {
            if(str[s-2]==' '||
              str[s-2]==',')conf=1;
          }
      }
    }
    if(starting||conf)
    {
      //CAPITALIZE
      if(str[s]>='a'&&str[s]<='z')
      {
        str[s]-='a';
        str[s]+='A';
      }
      switch(str[s])
      {
      case (char)129:str[s]=(char)154;break;
      case (char)164:str[s]=(char)165;break;
      case (char)132:str[s]=(char)142;break;
      case (char)134:str[s]=(char)143;break;
      case (char)130:str[s]=(char)144;break;
      case (char)148:str[s]=(char)153;break;
      case (char)135:str[s]=(char)128;break;
      case (char)145:str[s]=(char)146;break;
      }
      starting=false;
    }
  }
}

void capitalize_string_first_word(std::string &str){

  bool starting=true;
  int32_t bracket_count=0;
  char conf;
  int32_t s;
  for(s=0;s<str.length();s++){

    conf=0;
    if(str[s]=='['){++bracket_count;continue;}
    if(str[s]==']'){--bracket_count;continue;}
    if(bracket_count>0)continue;
    if(!starting)
    {
      if(str[s-1]==' '||
        str[s-1]=='\"')conf=1;
        if(str[s-1]=='\'')
        {
        //DISCOUNT SINGLE QUOTE IF IT ISN'T PRECEDED BY SPACE, COMMA OR NOTHING
          if(s<=0)conf=1;
          else if(s>=2)
          {
            if(str[s-2]==' '||
              str[s-2]==',')conf=1;
          }
      }
    }
    if(starting||conf)
    {
      //CAPITALIZE
      if(str[s]>='a'&&str[s]<='z')
      {
        str[s]-='a';
        str[s]+='A';
        return;
      }
      switch(str[s])
      {
      case (char)129:str[s]=(char)154;return;
      case (char)164:str[s]=(char)165;return;
      case (char)132:str[s]=(char)142;return;
      case (char)134:str[s]=(char)143;return;
      case (char)130:str[s]=(char)144;return;
      case (char)148:str[s]=(char)153;return;
      case (char)135:str[s]=(char)128;return;
      case (char)145:str[s]=(char)146;return;
      }
      if(str[s]!=' '&&str[s]!='\"')return;
      starting=false;
    }
  }
}

static void abbreviate_string_helper(std::string &str, int len) {
 if(str.length()>=2)
 {
  if((str[0]=='A'||str[0]=='a')&&
    str[1]==' ')
  {
    str.erase(str.begin()+1);
    str.erase(str.begin());

    if(str.length()<=len)return;
  }

  if(str.length()>=3)
  {
    if((str[0]=='A'||str[0]=='a')&&
      (str[1]=='N'||str[1]=='n')&&
      str[2]==' ')
    {
      str.erase(str.begin()+2);
      str.erase(str.begin()+1);
      str.erase(str.begin());

      if(str.length()<=len)return;
    }

    if(str.length()>=4)
    {
      if((str[0]=='T'||str[0]=='t')&&
        (str[1]=='H'||str[1]=='h')&&
        (str[2]=='E'||str[2]=='e')&&
        str[3]==' ')
      {
        str.erase(str.begin()+3);
        str.erase(str.begin()+2);
        str.erase(str.begin()+1);
        str.erase(str.begin());

        if(str.length()<=len)return;
      }
    }
  }
}

int32_t l;
for(l=(int32_t)str.length()-1;l>=1;l--)
{
  if(str[l-1]==' ')continue;

  if(str[l]=='a'||
    str[l]=='e'||
    str[l]=='i'||
    str[l]=='o'||
    str[l]=='u'||
    str[l]=='A'||
    str[l]=='E'||
    str[l]=='I'||
    str[l]=='O'||
    str[l]=='U')
  {
    str.erase(str.begin()+l);
    if(str.length()<=len)return;
  }
}

if(str.length()>len)str.resize(len);
}


void abbreviate_string(std::string &str, int32_t len){

  /*
  if (ttf_manager.ttf_active()) {
    // We'll need to use TTF-aware text shrinking.
    while (ttf_manager.size_text(str) > len)
      abbreviate_string_helper(str, (int32_t)str.length() - 1);
  } else */if(str.length()>len){
    // 1 letter = 1 tile.
  abbreviate_string_helper(str, len);
}
}



void get_number(int32_t number,std::string &str){
  
  str.erase();

  if(number<0)
  {
    number*=-1;
    str="negative ";
  }
  switch(number)
  {
  case 0:str="zero";break;
  case 1:str="one";break;
  case 2:str="two";break;
  case 3:str="three";break;
  case 4:str="four";break;
  case 5:str="five";break;
  case 6:str="six";break;
  case 7:str="seven";break;
  case 8:str="eight";break;
  case 9:str="nine";break;
  case 10:str="ten";break;
  case 11:str="eleven";break;
  case 12:str="twelve";break;
  case 13:str="thirteen";break;
  case 14:str="fourteen";break;
  case 15:str="fifteen";break;
  case 16:str="sixteen";break;
  case 17:str="seventeen";break;
  case 18:str="eighteen";break;
  case 19:str="nineteen";break;
  default:
    {
      if(number>=1000000000)
      {
        std::string nm;
        get_number(number/1000000000,nm);
        str+=nm;
        str+=" billion";
        if(number%1000000000!=0)
        {
          str+=" ";
          get_number(number%1000000000,nm);
          str+=nm;
        }
        return;
      }
      if(number>=1000000&&number<1000000000)
      {
        std::string nm;
        get_number(number/1000000,nm);
        str+=nm;
        str+=" million";
        if(number%1000000!=0)
        {
          str+=" ";
          get_number(number%1000000,nm);
          str+=nm;
        }
        return;
      }
      if(number>=1000&&number<1000000)
      {
        std::string nm;
        get_number(number/1000,nm);
        str+=nm;
        str+=" thousand";
        if(number%1000!=0)
        {
          str+=" ";
          get_number(number%1000,nm);
          str+=nm;
        }
        return;
      }
      if(number>=100&&number<1000)
      {
        std::string nm;
        get_number(number/100,nm);
        str+=nm;
        str+=" hundred";
        if(number%100!=0)
        {
          str+=" ";
          get_number(number%100,nm);
          str+=nm;
        }
        return;
      }
      if(number>=20&&number<100)
      {
        switch(number/10)
        {
        case 2:str="twenty";break;
        case 3:str="thirty";break;
        case 4:str="forty";break;
        case 5:str="fifty";break;
        case 6:str="sixty";break;
        case 7:str="seventy";break;
        case 8:str="eighty";break;
        case 9:str="ninety";break;
        }
        if(number%10!=0)
        {
          str+="-";
          std::string nm;
          get_number(number%10,nm);
          str+=nm;
        }
        return;
      }
      add_long_to_string(number,str);
      break;
    }
  }
}

void get_ordinal(int32_t number, std::string &str,bool shorten)
{
  str.erase();

  if(shorten)
  {
    if(number<0)
    {
      number*=-1;
      str="-";
    }
    add_long_to_string(number,str);
    switch(number%10)
    {
    case 1:
      if(number%100==11)str+="th";
      else str+="st";
      break;
    case 2:
      if(number%100==12)str+="th";
      else str+="nd";
      break;
    case 3:
      if(number%100==13)str+="th";
      else str+="rd";
      break;
    default:
      str+="th";
      break;
    }
    return;
  }


  if(number<0)
  {
    number*=-1;
    str="Negative ";
  }
  switch(number)
  {
  case 0:str="Zeroth";break;
  case 1:str="First";break;
  case 2:str="Second";break;
  case 3:str="Third";break;
  case 4:str="Fourth";break;
  case 5:str="Fifth";break;
  case 6:str="Sixth";break;
  case 7:str="Seventh";break;
  case 8:str="Eighth";break;
  case 9:str="Ninth";break;
  case 10:str="Tenth";break;
  case 11:str="Eleventh";break;
  case 12:str="Twelfth";break;
  case 13:str="Thirteenth";break;
  case 14:str="Fourteenth";break;
  case 15:str="Fifteenth";break;
  case 16:str="Sixteenth";break;
  case 17:str="Seventeenth";break;
  case 18:str="Eighteenth";break;
  case 19:str="Nineteenth";break;
  default:
    add_long_to_string(number,str);
    switch(number%10)
    {
    case 1:
      if(number%100==11)str+="th";
      else str+="st";
      break;
    case 2:
      if(number%100==12)str+="th";
      else str+="nd";
      break;
    case 3:
      if(number%100==13)str+="th";
      else str+="rd";
      break;
    default:
      str+="th";
      break;
    }
    break;
  }
}