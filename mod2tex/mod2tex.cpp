/******************************************************************************
 *
 *  mod2imp.cpp -	Utility to export a module in IMP format
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifdef _MSC_VER
#pragma warning( disable: 4251 )
#endif

#include <iostream>
#include <map>
#include <stdio.h>

#include <markupfiltmgr.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

using std::map;
using std::cout;
using std::endl;

#ifndef NO_SWORD_NAMESPACE

using namespace sword;

#endif


void usage(const char *progName, const char *error = 0) {
  if (error) fprintf(stderr, "\n%s: %s\n", progName, error);
  fprintf(stderr, "\n=== mod2imp (Revision $Rev$) SWORD module exporter.\n");
  fprintf(stderr, "\nusage: %s <module_name> [options]\n"
          "\t -s - strip markup instead of outputting raw native data.\n"
          "\t -f <option_name> <option_value> - when rendering (-r, above), option\n"
          "\t\tfilter values can be set with this option.\n\n"
          , progName);
  exit(-1);
}


int main(int argc, char **argv)
{
  // handle options
  if (argc < 2) usage(*argv);

  const char *progName   = argv[0];
  const char *modName    = argv[1];
  bool render            = true;
  bool strip             = false;
  SWBuf renderForm       = "PLAIN";
  SWBuf optionName;
  map<SWBuf, SWBuf> options; // optionName, optionValue;

  for (int i = 2; i < argc; i++) {
    if (!strcmp(argv[i], "-s")) {
      if (render) usage(progName, "-s can't be supplied when using -r");
      strip = true;
    }
    else if (!strcmp(argv[i], "-f")) {
      if (i+1 < argc) optionName          = argv[++i];
      if (i+1 < argc) options[optionName] = argv[++i];
      else usage(progName, "-f requires <option_name> <option_value>");
    }
    else usage(progName, (((SWBuf)"Unknown argument: ")+ argv[i]).c_str());
  }
  // -----------------------------------------------------

  MarkupFilterMgr *markupMgr = new MarkupFilterMgr(sword::FMT_PLAIN);

  SWMgr *mgr = (markupMgr) ? new SWMgr(markupMgr) : new SWMgr();

  // set any options filters passed with -f
  for (map<SWBuf, SWBuf>::iterator it = options.begin(); it != options.end(); it++) {
    mgr->setGlobalOption(it->first, it->second);
  }

  SWModule *module = mgr->getModule(modName);

  if (!module) usage(progName, (((SWBuf) "Couldn't find module: ") + modName).c_str());


  SWKey *key = module->getKey();
  VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);

  if (vkey)
    vkey->setIntros(true);

  char book = -1; // init nonexistent value
  
  for ((*module) = TOP; !module->popError(); (*module)++) {
    if (!vkey->getBook()) { continue; }
    vkey = SWDYNAMIC_CAST(VerseKey, module->getKey());
    if (book != vkey->getBook()) {
      std::cout << "\\kniha{" << vkey->getBookName() << "}" << std::endl;
      std::cout << "\\zkratka{" << vkey->getBookAbbrev() << "}" << std::endl;
      book = vkey->getBook();
    }
    if (!vkey->getChapter()) { continue; }
    if (!vkey->getVerse()) { continue; }
    std::cout << '<' << vkey->getChapter() << ':' << vkey->getVerse() << ' ' << ((render) ? module->renderText().c_str() : (strip) ? module->stripText() : module->getRawEntry()) << '>' << std::endl;
  }

  cout << endl;

  return 0;
}
