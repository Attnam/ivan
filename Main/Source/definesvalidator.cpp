/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

#include <algorithm>
#include <cstdarg>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <bitset>
#include <ctime>

#include "proto.h"
#include "devcons.h"
#include "game.h"
#include "message.h"

//static void DefinesValidatorAppend(std::string s);
//static void DefinesValidatorTop();
//static void DefinesValidatorAppendCode(std::string s);
std::ofstream DefinesValidator;
void DefinesValidatorAppend(std::string s)
{
  static std::stringstream ssValidateLine;ssValidateLine.str(std::string());ssValidateLine.clear(); //actually clear/empty it = ""

  ssValidateLine << s << std::endl;

  static bool bDummyInit = [](){
    DefinesValidator.open(
        festring(game::GetStateDir() + "definesvalidator.h").CStr(),
        std::ios::binary);
    return true;}();

  DefinesValidator.write(ssValidateLine.str().c_str(),ssValidateLine.str().length());
}
void DefinesValidatorTop()
{
  DefinesValidatorAppend("/****");
  DefinesValidatorAppend(" * AUTO-GENERATED CODE FILE, DO NOT MODIFY as modifications will be overwritten !!!");
  DefinesValidatorAppend(" *");
  DefinesValidatorAppend(" * After it is generated, update the one at source code path with it and");
  DefinesValidatorAppend(" * recompile so the results on the abort message (if happens) will be updated !!!");
  DefinesValidatorAppend(" */");
  DefinesValidatorAppend("");
  DefinesValidatorAppend("#ifndef _DEFINESVALIDATOR_H_");
  DefinesValidatorAppend("#define _DEFINESVALIDATOR_H_");
  DefinesValidatorAppend("");

  #define INCDEPS(dep) DefinesValidatorAppend("#include <"#dep">");
  INCDEPS(string);
  INCDEPS(algorithm);
  INCDEPS(cstdarg);
  INCDEPS(string);
  INCDEPS(sstream);
  INCDEPS(iostream);
  INCDEPS(vector);
  INCDEPS(bitset);
  INCDEPS(ctime);

  DefinesValidatorAppend("");

  #define INCDEPSH(dep) DefinesValidatorAppend("#include \""#dep"\"");
  INCDEPSH(error.h)

  DefinesValidatorAppend("");
  DefinesValidatorAppend("class definesvalidator{");
  DefinesValidatorAppend("");
  DefinesValidatorAppend(" public:");
  DefinesValidatorAppend("  static void init();");
  DefinesValidatorAppend("  static void DevConsCmd(std::string);");
  DefinesValidatorAppend("  static void GenerateDefinesValidator(std::string);");
  DefinesValidatorAppend("");
  DefinesValidatorAppend(" static void Validate() {");
  DefinesValidatorAppend("  std::stringstream ssErrors;");
  DefinesValidatorAppend("  std::bitset<32> bsA, bsB;");
  DefinesValidatorAppend("");
}
void DefinesValidatorAppendCode(std::string sDefineId, long valueReadFromDatFile)
{
  static std::stringstream ssMsg;ssMsg.str(std::string());ssMsg.clear(); //actually clear/empty it = ""

  ssMsg << "\"Defined " << sDefineId << " with value " << valueReadFromDatFile << " from .dat file " <<
    "mismatches hardcoded c++ define value of \" << " << sDefineId << " << \"!\"";


  static std::stringstream ssCode;ssCode.str(std::string());ssCode.clear(); //actually clear/empty it = ""

//  "  if( " << valueReadFromDatFile << " != ((ulong)" << sDefineId << ") ) // DO NOT MODIFY!" << std::endl <<
  ssCode <<
    "  " << std::endl <<
    "#ifdef " << sDefineId << " // DO NOT MODIFY!" << std::endl <<
    "  bsA = " << valueReadFromDatFile << ";" << std::endl <<
    "  bsB = " << sDefineId << ";" << std::endl <<
    "  if(bsA!=bsB)" << std::endl <<
    "    ssErrors << " << ssMsg.str() << " << std::endl;" << std::endl <<
    "#endif " << std::endl;


  DefinesValidatorAppend(ssCode.str());
}
void DefinesValidatorClose(){
  DefinesValidatorAppend("");
  DefinesValidatorAppend("  if(ssErrors.str().length() > 0) ABORT(ssErrors.str().c_str());");
  DefinesValidatorAppend("");
  DefinesValidatorAppend("}};");
  DefinesValidatorAppend("");
  DefinesValidatorAppend("#endif // _DEFINESVALIDATOR_H_");

  DefinesValidator.close();
}
#include "definesvalidator.h" //tip: 1st run this was commented
void CmdDevConsGenDefVal(std::string strOpt){
  definesvalidator::GenerateDefinesValidator(strOpt);
}
void definesvalidator::init(){
  devcons::AddDevCmd("DefVal", CmdDevConsGenDefVal,
    "<generate|validate> generate the validator at user config path or validate the file 'define.dat' (may abort)");
}
void definesvalidator::GenerateDefinesValidator(std::string strOpt)
{
  if(strOpt=="generate"){
    DefinesValidatorTop();

    for(const valuemap::value_type& p : game::GetGlobalValueMap())
      DefinesValidatorAppendCode(p.first.CStr(), p.second);

    DefinesValidatorClose();
    ADD_MESSAGE("generated the defines validator");
  }else
  if(strOpt=="validate"){
    definesvalidator::Validate(); //tip: 1st run this was commented
    ADD_MESSAGE("validated 'defines.dat'");
  }else{
    ADD_MESSAGE("invalid option: '%s'",strOpt.c_str());
  }
}
