#include "catalog.h"
#include "query.h"


/*
 * Inserts a record into the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Insert(const string & relation, 
	const int attrCnt, 
	const attrInfo attrList[])
{
/*  Insert a tuple with the given attribute values (in attrList) in relation. The value of the attribute is supplied in the attrValue member of the attrInfo structure. Since the order of the attributes in attrList[] may not be the same as in the relation, you might have to rearrange them before insertion. If no value is specified for an attribute, you should reject the insertion as Minirel does not implement NULLs.
 */
    
    
    //get relation
    //get order of attributes in relation
    //rearrange attribute list to match order of attributes in relation
    //insertfilescan of record created using the rearranged attribute list
    //end

    Status status;
    RelDesc rd;
    AttrDesc *attrs;
    int attrCnt;
    
    attrInfo sortedAttrList[attrCnt];
    
    // get relation data
    if ((status = relCat->getInfo(relation, rd)) != OK) return status;
    
    // get attribute data
    if ((status = attrCat->getRelInfo(rd.relName, attrCnt, attrs)) != OK)
        return status;
    
    for(int i; i < attrCnt; i++)
    {
        for(int j; j < attrCnt; j++)
        {
            if(attrs[i].attrName == attrList[j].attrName)
            {
                memcpy(sortedAttrList[i], attrList[j]);
            }
        }
    }
    
    //Unfinished.  Still need to create record and insert it.
    
    
// part 6
return OK;

}

