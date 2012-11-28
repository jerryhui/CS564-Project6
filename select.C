#include "catalog.h"
#include "query.h"
#include "stdio.h"
#include "stdlib.h"


// forward declaration
const Status ScanSelect(const string & result, 
			const int projCnt, 
			const AttrDesc projNames[],
			const AttrDesc *attrDesc, 
			const Operator op, 
			const char *filter,
			const int reclen);

/*
 * Selects records from the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 *
 * LOG:
 * 2012/11/28 JH: First implementation of overhead info collection.
 */

const Status QU_Select(const string & result, 
		       const int projCnt, 
		       const attrInfo projNames[],
		       const attrInfo *attr, 
		       const Operator op, 
		       const char *attrValue)
{
    // Qu_Select sets up things and then calls ScanSelect to do the actual work
    cout << "Doing QU_Select " << endl;

    // reject invalid format
    if (attr->attrType <0 || attr->attrType>2) return BADCATPARM;
    
    Status status;
    AttrDesc *attrDesc;
    AttrDesc projAttrDesc[projCnt];
    int attrCnt,i,reclen,searchAttr;
    
    // get info of all attributes in requested relation
    if ( (status=attrCat->getRelInfo(attr->relName, attrCnt, attrDesc)) != OK)
        return status;
    
    // get info of all attributes to project
    for (i=0; i<projCnt; i++) {
        status = attrCat->getInfo(projNames[i].relName, projNames[i].attrName,projAttrDesc[i]);
        if (status!=OK)
            return status;
    }
    
    // loop through attributes
    //  get attribute info about the attr to search for
    //  calculate length of a record
    reclen=0;
    for (i=0; i<attrCnt; i++) {
        reclen += attrDesc[i].attrLen;
        if (strcmp(attrDesc[i].attrName,attr->attrName) == 0)
            searchAttr=i;
    }
    
    // pass info along to ScanSelect for the actual work
    return ScanSelect(result, projCnt, projAttrDesc,
                      &attrDesc[searchAttr], op, attrValue, reclen);
}


const Status ScanSelect(const string & result, 
			const int projCnt,
			const AttrDesc projNames[],
			const AttrDesc *attrDesc, 
			const Operator op, 
			const char *filter,
			const int reclen)
{
    cout << "Doing HeapFileScan Selection using ScanSelect()" << endl;
    
    // open result heap file (with InsertScan)
    // open relation heap file (with HeapFileScan)
    
    
    // sequentially go through heap file
    //      if current record matches search criteria
    //          store into "result"

    return OK;
}
