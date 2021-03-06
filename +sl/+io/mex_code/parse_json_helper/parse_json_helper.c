/*
 *
 * Status: This code is currently unfinished.
 *
 */

/*
 um = mendeley.user_manager.getInstance;
user = um.getUser(1);
dm = mendeley.library.raw_doc_manager.getInstance(user);
rdm = dm;
raw_doc_data = rdm.doc_data;
n_raw_docs   = length(raw_doc_data);
parsed_json_local = cell(1,n_raw_docs);
iEntry = 1
cur_raw_doc_data = raw_doc_data{iEntry};
temp = sl.io.parseJSON(cur_raw_doc_data);
 */


#include "matrix.h"
#include "mex.h"

//mex parse_json_helper.c


void mexFunction( int nlhs, mxArray *plhs[],
        int nrhs, const mxArray*prhs[] )
{
    
    // [is_string,is_esc_char,is_quote] = parse_json_helper(string)
    
    //see mxmalloc for example ...
    
    char* string;
    size_t string_byte_length;
    mwSize string_length;
    mwSize index;
    mxLogical* is_string;
    mxLogical* is_esc_char;
    mxLogical* is_quote;
    bool in_string = false;
    
    
    //This seems wasteful, I thought I was working with the string
    //directly ... Might be able to improve this ...
    string_byte_length = mxGetNumberOfElements(prhs[0])*sizeof(mxChar)+1;
    string_length = (mwSize)mxGetNumberOfElements(prhs[0]);
    string        = mxMalloc(string_length);
    mxGetString(prhs[0],string,(mwSize)string_length);
    
    //NOTE: This initializes everything to false, we could eventually remove the initialization
    plhs[0] = mxCreateLogicalMatrix(1,string_length);
    is_string = mxGetLogicals(plhs[0]);
    plhs[1] = mxCreateLogicalMatrix(1,string_length);
    is_esc_char = mxGetLogicals(plhs[1]);
    plhs[2] = mxCreateLogicalMatrix(1,string_length);
    is_quote = mxGetLogicals(plhs[2]);
    
    //TODO: Add check on empty string and return ...
    
    //NOTE: We assume the first character is not a quote
    //as this would be invalid JSON
    
    //is_string[0]   = false;
    //is_esc_char[0] = false;
    //is_quote[0]    = false;
    
    /* Copy data into the mxArray */
    for ( index = 1; index < string_length; index++ ) {
        if (!is_esc_char[index-1])
        {
            if (string[index] == '"')
            {      //'"'
                is_quote[index] = true;
                in_string = !in_string;
            }
            else if (string[index] == '\\') //'\'
            {   
                is_esc_char[index] = true;
                is_string[index]   = in_string;
            }
            else 
            {
                is_string[index] = in_string;
            }
        } 
        else 
        {
            is_string[index] = in_string;
        }
    }
    
    mxFree(string);
    
}