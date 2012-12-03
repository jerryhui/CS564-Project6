#include "catalog.h"
#include "query.h"


/*
 * Deletes records from a specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 *
 * Log:
 * 2012/11/26 JH: First implementation.
 * 2012/12/03 JH: Condition value conversion added. Can delete all tuples.
 */
const Status QU_Delete(const string & relation, 
		       const string & attrName, 
		       const Operator op,
		       const Datatype type, 
		       const char *attrValue)
{
    // rejects when no relation or attribute is given
    if (relation.empty()) return BADCATPARM;
    
    Status status;
    RID rid;
    HeapFileScan *hfs = new HeapFileScan(relation, status);
    if (status!=OK) return status;
    
    // find attribute info to set up scan
    AttrDesc aDesc;
    void* filter;
    int iVal;
    float fVal;
    
    if (attrName!="") {
        switch (type) {
            case STRING: filter = (void*)attrValue; break;
            case INTEGER: {
                iVal = atoi(attrValue);
                filter = &iVal;
                break;
            }
            case FLOAT: {
                fVal = atof(attrValue);
                filter = &fVal;
                break;
            }
        }
        status = attrCat->getInfo(relation, attrName, aDesc);
        if (status!=OK) return status;
        hfs->startScan(aDesc.attrOffset, aDesc.attrLen, type, (char*)filter, op);
    } else
        hfs->startScan(0, 0, type, NULL, op);
    
    // if error occurs during startScan, the loop below is skipped and exit steps are taken
    
    // search for records that match given condition
    while (status==OK) {
        status = hfs->scanNext(rid);
        if (status == FILEEOF) break;
        
        hfs->deleteRecord();
    }
    
    // delete record
    delete hfs;
    return OK;
}