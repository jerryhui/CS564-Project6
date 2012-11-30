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
 * 2012/11/28 JH: First implementation.
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

    Status status;
    AttrDesc *attrDesc;
    AttrDesc projAttrDesc[projCnt];
    int attrCnt,i,j,reclen,searchAttr;
    
    // reject invalid format
    if ( (attr != NULL)
        && (attr->attrType <0 || attr->attrType>2) )
        return BADCATPARM;

    // get info of all attributes in requested relation
    if ( (status=attrCat->getRelInfo(projNames[0].relName, attrCnt, attrDesc)) != OK)
        return status;
    
    reclen=0;
    for (i=0; i<attrCnt; i++) {
        for (j=0; j<projCnt; j++)
            if (strcmp(attrDesc[i].attrName,projNames[j].attrName)==0) {
                // if current attribute is part of projection:
                // 1) calculate length of a record
                // 2) fill in projection attr desc
                reclen += attrDesc[i].attrLen;
                projAttrDesc[j] = attrDesc[i];
            }
        if (attr!=NULL && strcmp(attrDesc[i].attrName,attr->attrName) == 0)
            searchAttr=i;
    }
    
    // pass info along to ScanSelect for the actual work
    return ScanSelect(result, projCnt, projAttrDesc,
                      &attrDesc[searchAttr],
                      op, attrValue, reclen);
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
    
    Status status;
    RID rid;
    Record rec;
    int i;
    Datatype t;
    
    Record resultRec;
    char outputData[reclen];
    resultRec.data = (void*)outputData;
    resultRec.length = reclen;
    int outputOffset;
    
    // open relation file to read, result file to write
    InsertFileScan resultFile(result,status);
    if (status!=OK) return status;
    HeapFileScan relFile(projNames[0].relName,status);
    if (status!=OK) return status;

    // set up scan condition
    if (attrDesc!=NULL) {
        switch (attrDesc->attrType) {
            case 1: t = INTEGER; break;
            case 2: t = FLOAT; break;
            case 0: t = STRING; break;
        }
        status=relFile.startScan(attrDesc->attrOffset,
                                 attrDesc->attrLen,
                                 t, filter, op);
    } else {
        // no condition is given, do an unconditional scan
        status=relFile.startScan(0,0,STRING, NULL, EQ);
    }
        
    if  ( status != OK ) return status;
    
    while (relFile.scanNext(rid) == OK) {
        // current record matches search criteria

        // get record
        if ( (status = relFile.getRecord(rec)) != OK)
            return status;
        
        // 1) create new record from projection
        outputOffset = 0;
        for (i=0; i<projCnt; i++) {
            memcpy(outputData + outputOffset,
                   (char*)rec.data + projNames[i].attrOffset,
                   projNames[i].attrLen);
            outputOffset += projNames[i].attrLen;
        }
        
        // 2) store into result file
        RID outRID;
        status = resultFile.insertRecord(resultRec, outRID);
        if (status!=OK)
            return status;
    }

    return OK;
}
