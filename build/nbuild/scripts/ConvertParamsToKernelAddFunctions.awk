BEGIN   { i=0 }
NF != 0 {
         if(match($0, /\*/))
         { 
          print "    //"
          print "    p = kernel_add_ptr_arg(p, args["i"].value);"
          print "" 
         }
         else if(match($0,/int/))
         {
          print "    //"
          print "    p = kernel_add_int_arg(p, args["i"].value);"
          print ""
         }
         else if(match($0,/char/))
         {
          print "    //"
          print "    p = kernel_add_char_arg(p, args["i"].value);"
          print ""
         }
         else
         {
          print "#error Unhandled type in ConvertParamsToKernelAddFunctions.awk"
         }
         
         ++i;
        }