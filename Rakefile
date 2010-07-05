# 
# MAKE SUCKS
#
# rake <run>
#   will execute src/main.c just like a script language. 
# 
# -FLeurer<me.ssword@gmail.com>
#

CFILES  = Dir.glob('src/*.c').join(' ')
OFiles  = CFiles.map{|fn_c| 'bin/'+File.basename(fn_c).ext('o') }

CFlag   = '-Wall'
CInc    = "-Isrc -Ivendor/gc/include -Ivendor"
LIB_GC  = 'vendor/gc/.libs/libgc.a'


###############################
# compile -> link -> run 
# automatic work

task :run => 'bin/main' do
  puts '-'*50
  sh "bin/main"
end

task :default => :run

################################
# link all together

file 'bin/main' => (OFiles+[LIB_GC]) do
  sh "gcc #{CFlag} #{OFiles*' '} #{LIB_GC} -o bin/main"
end



#################################################
# compile all the C stuff, and gc

file LIB_GC do
  sh "(cd vendor/gc && ./configure --disable-threads -q && make -s)"
end

CFiles.each do |fn_c|
  fn_o = 'bin/'+File.basename(fn_c).ext('o')
  file fn_o => [fn_c] do
    sh "gcc #{CFlag} #{CInc} -c #{fn_c} -o #{fn_o}"
  end
end



