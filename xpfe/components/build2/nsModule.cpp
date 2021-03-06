/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

// minotaur specific stuff that will get smaller and smaller until it shrinks to zero. 

#include "nsIGenericFactory.h"
#include "nsICategoryManager.h"
#include "nsServiceManagerUtils.h"
#include "rdf.h"
#include "nsXPIDLString.h"
#include "nsCharsetMenu.h"
#include "nsFontPackageHandler.h"
#include "nsWindowDataSource.h"
#include "nsRDFCID.h"
#include "nsAutoComplete.h"

#if defined(MOZ_LDAP_XPCOM)
#include "nsLDAPAutoCompleteSession.h"
#endif

#if defined(XP_WIN)
#include "nsWindowsHooks.h"
#endif // Windows

#include "nsCURILoader.h"

// Factory constructors
NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(nsWindowDataSource, Init)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsAutoCompleteItem)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsAutoCompleteResults)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsFontPackageHandler)

#if defined(MOZ_LDAP_XPCOM)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsLDAPAutoCompleteSession)
#endif

#if defined(XP_WIN)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsWindowsHooks)
#endif // Windows

static NS_METHOD
RegisterWindowDS(nsIComponentManager *aCompMgr,
                 nsIFile *aPath,
                 const char *registryLocation,
                 const char *componentType,
                 const nsModuleComponentInfo *info)
{
    nsresult rv;
    nsCOMPtr<nsICategoryManager> catman = do_GetService(NS_CATEGORYMANAGER_CONTRACTID, &rv);
    if (NS_FAILED(rv)) return rv;

    return catman->AddCategoryEntry("app-startup", "Window Data Source",
                                    "service," NS_RDF_DATASOURCE_CONTRACTID_PREFIX "window-mediator",
                                    PR_TRUE, PR_TRUE, nsnull);
    return NS_OK;
}

static const nsModuleComponentInfo components[] = {
    { "AutoComplete Search Results", NS_AUTOCOMPLETERESULTS_CID, NS_AUTOCOMPLETERESULTS_CONTRACTID,
      nsAutoCompleteResultsConstructor},
    { "AutoComplete Search Item", NS_AUTOCOMPLETEITEM_CID, NS_AUTOCOMPLETEITEM_CONTRACTID,
      nsAutoCompleteItemConstructor},  

#if defined(MOZ_LDAP_XPCOM)
    { "LDAP Autocomplete Session", NS_LDAPAUTOCOMPLETESESSION_CID,
	  "@mozilla.org/autocompleteSession;1?type=ldap",
	  nsLDAPAutoCompleteSessionConstructor },
#endif 

    { "nsCharsetMenu", NS_CHARSETMENU_CID,
      NS_RDF_DATASOURCE_CONTRACTID_PREFIX NS_CHARSETMENU_PID,
      NS_NewCharsetMenu },
    { "nsFontPackageHandler", NS_FONTPACKAGEHANDLER_CID,
      "@mozilla.org/locale/default-font-package-handler;1",
      nsFontPackageHandlerConstructor },
    { "nsWindowDataSource",
      NS_WINDOWDATASOURCE_CID,
      NS_RDF_DATASOURCE_CONTRACTID_PREFIX "window-mediator",
      nsWindowDataSourceConstructor, RegisterWindowDS },
};

NS_IMPL_NSGETMODULE(application, components)
