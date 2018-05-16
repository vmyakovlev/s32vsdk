BEGIN   { i=0 }
NF != 0 {
         if(match($0, /_local/)) {print 0}
         else if(match($0, /_global/)) {print 1}
         else if(match($0, /_constant/)) {print 3}
         else {print 2}
         ++i; 
        }
END     { 
         if(i < 8) 
         {
          while(i<8) 
          { 
           print 0;
           ++i;
          } 
         }  
        }
