CFILES  = Dir.glob('src/*.c').join(' ')
OFILES  = Dir.glob('*.o').join(' ')
FLAGS   = '-Wall'

INCS    = "-Isrc -Ivendor/gc/include -Ivendor"
LIB_GC  = 'vendor/gc/.libs/libgc.a'
LIBS    = LIB_GC 

task :build => :build_libs do
    # if gc lib is not currently compiled
    CFILES.split(' ').select{|fn| uptodate?(fn, fn.gsub(/src\//,'').ext('o')) }.each{|fn|
        #sh "gcc #{FLAGS} #{INCS} -c #{fn} 2>&1 | grep '[[:digit:]]*: \\(error\\|warning\\)' --color=auto 1>&2"
        sh "gcc #{FLAGS} #{INCS} -c #{fn}"
    }
end

task :build_libs => [:build_gc]

task :build_gc do 
    if not File.exist?(LIB_GC)
        sh "cd vendor/gc && ./configure --disable-threads -q && make -s"
    end
end

task :link => [:build] do
    sh "gcc #{FLAGS} #{OFILES} #{LIBS} -o bin/main"
end

task :run => [:build, :link] do 
    puts '-'*50
    sh "bin/main"
end

task :clean do
    `rm *.o`
end

