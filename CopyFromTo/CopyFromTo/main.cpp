#include "tchar.h"
#include <iostream>
#include "windows.h"
#include "pugixml.hpp"
#include <errno.h>

using namespace std;
using namespace pugi;

void main () {

	xml_document doc;

	xml_parse_result result = doc.load_file (_T("data.xml"));

	if (result) {

		xml_node tagRoot = doc.child (_T("Data"));
		
		for (xml_node tagCopy = tagRoot.child (_T("Copy")); tagCopy; tagCopy = tagCopy.next_sibling (_T("Copy")))
		{
			TCHAR *fTo = (TCHAR *) tagCopy.attribute (_T("To")).value ();
			TCHAR *fFrom = (TCHAR *) tagCopy.attribute (_T("From")).value ();
			
			if (CopyFileW (fFrom, fTo, FALSE) == 0) {
				cout << "Error copy file." << endl;
			} else {
				cout << "File copied." << endl;
			}
		}
	
	} else cout << "Load result: " << result.description () << std::endl;

	cin.get ();
}