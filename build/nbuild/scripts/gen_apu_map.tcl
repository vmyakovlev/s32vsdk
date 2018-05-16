
# 
puts stderr "===================================================="
puts stderr " GEN_APU_MAP"
puts stderr  $tcl_version
package  require tdom
puts stderr "----------------------------------------------------"




##########################################################################

# Open XML file
set            fp [open [file join [lindex $argv 0]]]
set xml [read $fp]
close         $fp

set doc [dom parse $xml]
set root [$doc documentElement]

# FUNCTION SYMBOLS
# \todo: MISSING A PRETTY / CLEAN FUNCTION SIGNATURE
proc explore_functions { root } {

set flist [$root selectNodes {/memorymap/memory/function}]
set fnbr 0

puts " // FUNCTION NAME, ADDRESS, FUNCTION SIZE, FUNCTION STACK SIZE (BYTES)"
foreach fnode $flist {
   set type  [ $fnode nodeType]
   set name  [ $fnode nodeName]
   
   set mnode [ $fnode parentNode ]
   set mname [ $mnode getAttribute name]
   
   set fname [ $fnode getAttribute name]
   set faddr [ $fnode getAttribute from_addr]
   set faend [ $fnode getAttribute to_addr]
   set fsize [ expr {$faend-$faddr+1} ]
   # Put a default value in case it does not exist 
   #   (like assembler functions)
   set fstck [ $fnode getAttribute stack_frame_size 0 ]
   set fsect [ $fnode getAttribute section ]
   set fobjf [ $fnode getAttribute object_file ]
   set fbind [ $fnode getAttribute symbol_binding ]
   
   set fdisp \"
   append fdisp $fname \"
   
   set fmsec $mname
   append fmsec $fsect
   
   # puts [ format { { %-120s, 0x%05X, %4i, %4i }, // [%3i][%-12s]: %s} $fdisp $faddr $fsize $fstck $fnbr $fmsec $fnode ] 
   puts [ format { { %-120s, 0x%05X, %4i, %4i }, // [%3i][%-12s]} $fdisp $faddr $fsize $fstck $fnbr $fmsec ] 
   incr fnbr
}
}

# STACK SYMBOLS
proc explore_stacks { root } {

set slist [$root selectNodes {/memorymap/memory/stack}]
set snbr 0

puts " // STACK NAME, ADDRESS, STACK SIZE"
foreach snode $slist {

   set type  [ $snode nodeType]
   set name  [ $snode nodeName]

   set mnode [ $snode parentNode ]
   set mname [ $mnode getAttribute name]
   
   set sname $mname
   append sname ::stack 

   set sdisp \"
   append sdisp $sname \"
   
   set saddr [ $snode getAttribute from_addr]
   set saend [ $snode getAttribute to_addr]
   set ssize [ expr {$saend-$saddr+1} ]
   
   # puts [ format { { %-40s, 0x%05X, %4i }, // [%3i][%-12s]: %s} $sdisp $saddr $ssize $snbr $mname $snode] 
   puts [ format { { %-40s, 0x%05X, %4i }, // [%3i][%-12s]} $sdisp $saddr $ssize $snbr $mname ] 
   
   incr snbr
}
}

# BSS/DATA SYMBOLS
proc explore_data { root p } {

set dlist [$root selectNodes $p ]
set dnbr 0

puts " // VARIABLE NAME, ADDRESS, SIZE"
foreach dnode $dlist {

   set type  [ $dnode nodeType]
   set name  [ $dnode nodeName]
   
   set dname [ $dnode getAttribute name ]

   if {[ string length dname ] > 0} {
   
      set mnode [ $dnode parentNode ]
      set mname [ $mnode getAttribute name]

      
      set daddr [ $dnode getAttribute from_addr]
      set daend [ $dnode getAttribute to_addr]
      set dsize [ expr {$daend-$daddr+1} ]

      set dsect [ $dnode getAttribute section ]
      set dobjf [ $dnode getAttribute object_file ]
      set dbind [ $dnode getAttribute symbol_binding ]

      set ddisp \"
      append ddisp $dname \"
      
      set dmsec $mname
      append dmsec $dsect
      
      # puts [ format { { %-40s, 0x%05X, %4i }, // [%3i][%-12s]: %s} $ddisp $daddr $dsize $dnbr $dmsec $dnode] 
      puts [ format { { %-40s, 0x%05X, %4i }, // [%3i][%-12s]} $ddisp $daddr $dsize $dnbr $dmsec] 
      
      incr dnbr
   }
}
}

# explore function
puts [ format {const apu_load_functions_info_t apu_load_functions[] = }]
puts "{"
explore_functions $root 
puts "};"

puts [ format {const apu_load_data_info_t apu_load_data[] = } ]
puts "{"
explore_stacks    $root 
explore_data      $root {/memorymap/memory/bss}
explore_data      $root {/memorymap/memory/data}
puts "};"

##########################################################################
# dom parse  $xml doc
# $doc documentElement root

proc explore {parent n} {
   set type [$parent nodeType]
   set name [$parent nodeName]
   if {$type == "ELEMENT_NODE"} {
      puts "$parent: <$n/$name> "
      if {[llength [$parent attributes]]} {
         puts "  attributes: [join [$parent attributes] ", "]"
         # foreach a [$parent attributes] {
         # puts "  [$a]"
         # }
      }
      foreach child [$parent childNodes] {
         explore $child "$n/$name"
      }
   } else { 
      puts "$parent: <$n/$name> "
      return 
   }
}


# <?xml version="1.0"?>
# <!-- Release: retargetable 12R1.20 -->
# <memorymap version="1.0" complete="1">
# <memory name="PM" size="8192" width="32" offset="0" used="4602">
#   <function from_addr="0x0" to_addr="0x1a5" object_file="Release/bmrk_main.o" section=".text" name="_main" symbol_binding="Global" stack_frame_size="1604">
#   </function>

# domNode0089FF28: ELEMENT_NODE <memorymap>
#  attributes: version, complete
# domNode009C0880: ELEMENT_NODE <memory>
#  attributes: name, size, width, offset, used
# domNode009C09F8: ELEMENT_NODE <stack>
#  attributes: from_addr, to_addr
# domNode009C0A90: ELEMENT_NODE <memory>
#   attributes: name, size, width, offset, used
# domNode009C0BB8: ELEMENT_NODE <memory>
#   attributes: name, size, width, offset, used
# domNode009C0CE0: ELEMENT_NODE <memory>
#   attributes: name, size, width, offset, used
# domNode009C0EE8: ELEMENT_NODE <function>
#  attributes: from_addr, to_addr, object_file, section, name, symbol_binding, stack_frame_size
# domNode009C1080: ELEMENT_NODE <function>
#   attributes: from_addr, to_addr, object_file, section, name, symbol_binding, stack_frame_size
# domNode009C1240: ELEMENT_NODE <function>
#   attributes: from_addr, to_addr, object_file, section, name, symbol_binding, stack_frame_size
# domNode009C13F8: ELEMENT_NODE <function>

# domNode0089FF28: <memorymap>
#   attributes: version, complete
# domNode009C4888: <memory>
#   attributes: name, size, width, offset, used
# domNode009C4A00: <stack>
#   attributes: from_addr, to_addr
# domNode009C4A98: <memory>
#  attributes: name, size, width, offset, used
# domNode009C4BC0: <memory>
#   attributes: name, size, width, offset, used
# domNode009C4CE8: <memory>
#   attributes: name, size, width, offset, used
# domNode009C4EF0: <function>
#   attributes: from_addr, to_addr, object_file, section, name, symbol_binding, stack_frame_size
# domNode009C5088: <function>
#   attributes: from_addr, to_addr, object_file, section, name, symbol_binding, stack_frame_size
# domNode009C5248: <function>
#   attributes: from_addr, to_addr, object_file, section, name, symbol_binding, stack_frame_size
  
# OK, ugly
# explore $root " "



# FUNCTION SYMBOLS
# Extract function information
# need to cleanup function name...

proc explore_function_old { mnode } {

set mname [$mnode getAttribute name ]
set flist [$mnode selectNodes {/memorymap/memory/function}]
set fcnt [ llength $flist ]

puts $mname
puts $fcnt 

# if {$fcnt > 0} {

# foreach fnode $flist {
   # set type  [ $fnode nodeType]
   # set name  [ $fnode nodeName]
   
   # set fname [ $fnode getAttribute name]
   # set faddr [ $fnode getAttribute from_addr]
   # set faend [ $fnode getAttribute to_addr]
   # set fsize [ expr {$faend-$faddr+1} ]
   # set fstck [ $fnode getAttribute stack_frame_size ]
   # set fsect [ $fnode getAttributesection ]
   # set fobjf [ $fnode getAttributeobject_file ]
   # set fbind [ $fnode getAttributesymbol_binding ]
   # puts [ format {\*%s*\:\[%-4s\]%-80s<%04X><%4i>[%4i]} $fnode $mname $fname $faddr $fsize $fstck ] 
# }
# }

}



# MEMORY SYMBOLS

# OK: NEED TO CHANGE STRATEGY...
# WE HAVE A BUNCH OF MEMORIES: 
# WE NEED TO USE THEM...
# OK: WE DONT CARE: THEY ALL COMING FROM A MEMORY: EASY TO TRACK DOWN
# set mlist [$root selectNodes {/memorymap/memory}]
# set msize [ llength $mlist ]
# puts $msize



# set f0 [lindex $flist 0]
# puts [$f0 nodeValue]
# puts "  attributes: [join [$parent attributes] ", "]"
 
# foreach f [functionlist] {
#    puts [$f nodeValue]
# } 
 
# ERROR: doesnt wotk  
# proc xmlpretty xml { [[dom parse $xml doc] documentElement] asXML }
# dom parse $xml memory
# set nodeList [$root selectNodes /memorymap/memory]


