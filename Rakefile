# 
# MAKE SUCKS
#
# rake <run>
#   will execute src/main.c just like a script language. 
# 
# -FLeurer<me.ssword@gmail.com>
#

CFiles  = Dir.glob('src/*.c')
OFiles  = CFiles.map{|fn_c| 'bin/'+File.basename(fn_c).ext('o') }

CFlag   = '-Wall'
CInc    = "-Isrc -Ilib/gc6.8/include -Ivendor"
LIB_GC  = 'lib/gc6.8/.libs/libgc.a'


###############################
# compile -> link -> run 
# automatic work

task :default => :run

task :run => 'bin/main' do
  puts '-'*50
  sh "bin/main"
end

task :clean do
  sh 'rm -rf bin/*'
end

################################
# link all together 

file 'bin/main' => (OFiles+[LIB_GC]) do
  sh "gcc #{CFlag} #{OFiles*' '} #{LIB_GC} -o bin/main"
end


#################################################
# compile all the C stuff, and gc

file LIB_GC do
  sh "(cd lib/gc6.8 && ./configure --disable-threads -q && make -s)"
end

CFiles.each do |fn_c|
  fn_o = 'bin/'+File.basename(fn_c).ext('o')
  file fn_o => [fn_c] do
    sh "gcc #{CFlag} #{CInc} -c #{fn_c} -o #{fn_o}"
  end
end

