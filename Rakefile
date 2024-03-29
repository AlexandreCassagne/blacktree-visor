require 'rake'

ROOT_DIR = File.expand_path('.')
SRC_DIR = File.join(ROOT_DIR, 'src')
TMP_DIR = File.join(ROOT_DIR, 'tmp')
XCODE_PROJECT = File.join(SRC_DIR, 'Visor.xcodeproj')
VISOR_BUNDLE = "Visor.bundle"
BUILD_DIR = File.join(SRC_DIR, 'build')
BUILD_RELEASE_DIR = File.join(TMP_DIR, 'build', 'Release')
BUILD_RELEASE_PATH = File.join(BUILD_RELEASE_DIR, VISOR_BUNDLE)
RELEASE_DIR = File.join(ROOT_DIR, 'releases')
VISOR_XIB = File.join(TMP_DIR, 'Visor.xib')
INFO_PLIST = File.join(TMP_DIR, 'Info.plist')
SIMBL_DIR = File.expand_path(File.join('~', 'Library', 'Application Support', 'SIMBL'))
SIMBL_PLUGINS_DIR = File.expand_path(File.join(SIMBL_DIR, 'Plugins'))
PUBLISH_FOLDER = File.expand_path(File.join("~", "Dropbox", "Public", "Visor"))

# http://kpumuk.info/ruby-on-rails/colorizing-console-ruby-script-output/
begin
  require 'Win32/Console/ANSI' if PLATFORM =~ /win32/
rescue LoadError
  raise 'You must "gem install win32console" to use terminal colors on Windows'
end

def colorize(text, color_code)
  "#{color_code}#{text}\e[0m"
end

def red(text); colorize(text, "\e[31m"); end
def green(text); colorize(text, "\e[32m"); end
def yellow(text); colorize(text, "\e[33m"); end
def blue(text); colorize(text, "\e[34m"); end
def magenta(text); colorize(text, "\e[35m"); end
def azure(text); colorize(text, "\e[36m"); end
def white(text); colorize(text, "\e[37m"); end
def black(text); colorize(text, "\e[30m"); end

def file_color(text); yellow(text); end
def dir_color(text); blue(text); end
def cmd_color(text); azure(text); end

def die(msg, status=1)
  puts red("Error[#{status||$?}]: #{msg}")
  exit status||$?
end

def version()
  $version = ENV["version"] || 'Custom'
end

def revision()
  $revision = `git rev-parse HEAD`.strip
  $short_revision = $revision[0...6]
end

def dirty_repo_warning()
  is_clean = `git status`.match(/working directory clean/)
  puts red("Repository is not clean! You should commit all changes before releasing.") unless is_clean
end

def patch(path, replacers)
  puts "#{cmd_color('Patching')} #{file_color(path)}"
  lines = []
  File.open(path, "r") do |f|
    f.each do |line|
      replacers.each do |r|
        line.gsub!(r[0], r[1])
      end
      lines << line
    end
  end
  File.open(path, "w") do |f|
    f << lines
  end
end

def sys(cmd)
  puts "> " + yellow(cmd)
  system(cmd)
end

desc "opens XCode project"
task :open do 
  `open "#{XCODE_PROJECT}"`
end

desc "builds project"
task :build do
  puts "#{cmd_color('Building')} #{file_color(XCODE_PROJECT)}"
  Dir.chdir(SRC_DIR) do
    `xcodebuild -configuration Release 1>&2`
    die("build failed") unless $?==0
  end
end

desc "prepares release"
task :release do
  puts "#{cmd_color('Checking environment ...')}"
  dirty_repo_warning()
  version()
  revision()
  mkdir_p(RELEASE_DIR) unless File.exists? RELEASE_DIR

  puts "#{cmd_color('Copying sources to temporary directory ...')}"
  `cp -r "#{SRC_DIR}" "#{TMP_DIR}"`
  `rm -rf "#{TMP_DIR}/build"` # this might cause troubles when doing from dev machine releases

  puts "#{cmd_color('Patching version info ...')}"
  patch(VISOR_XIB, [['**VERSION**', $version], ['**REVISION**', $short_revision]])
  patch(INFO_PLIST, [['**VERSION**', $version]])

  puts "#{cmd_color('Building')} #{file_color(TMP_DIR)}"
  Dir.chdir(TMP_DIR) do
    `xcodebuild -configuration Release 1>&2`
    die("build failed") unless $?==0
  end

  result = File.join(RELEASE_DIR, "Visor-#{$version}-#{$short_revision}.zip");
  puts "#{cmd_color('Zipping')} #{dir_color(result)}"
  Dir.chdir(BUILD_RELEASE_DIR) do
    unless system("zip -r \"#{result}\" Visor.bundle") then puts red('need zip on command line (download http://www.info-zip.org/Zip.html)') end;
  end
  Rake::Task["clean"].execute nil
end

desc "removes intermediate build files"
task :clean do
  puts "#{cmd_color('Removing')} #{dir_color(TMP_DIR)}"
  `rm -rf "#{TMP_DIR}"`
end

desc "removes all releases"
task :purge do
  puts "#{cmd_color('Removing')} #{dir_color(RELEASE_DIR)}"
  `rm -rf "#{RELEASE_DIR}"`
end

desc "installs newest release into ~/Library/Application Support/SIMBL/Plugins"
task :install do
  die("first build release> rake release version=1.6") unless File.exists? RELEASE_DIR
  zip = ""
  Dir.chdir(RELEASE_DIR) do
    files = `ls -1at *.zip`
    die("unable to locate any zip file in #{RELEASE_DIR}") unless $?==0
    zip = files.split("\n")[0].strip
    die("unable to locate any zip file in #{RELEASE_DIR}") if (zip=="")
    puts "#{cmd_color('Picked (latest)')} #{file_color(zip)}"
    sys("rm -rf \"#{VISOR_BUNDLE}\"") if File.exists? VISOR_BUNDLE # for sure
    puts "#{cmd_color('Unzipping into')} #{dir_color(SIMBL_PLUGINS_DIR)}"
    Dir.mkdir "#{SIMBL_DIR}" unless File.exists?(SIMBL_DIR)
    Dir.mkdir "#{SIMBL_PLUGINS_DIR}" unless File.exists?(SIMBL_PLUGINS_DIR)
    die("problem in unzipping") unless system("unzip \"#{zip}\"")
    dest = File.join(SIMBL_PLUGINS_DIR, VISOR_BUNDLE)
    sys("rm -rf \"#{dest}\"") if File.exists? dest
    die("problem in moving to SIMBL plugins. Do you have SIMBL installed? Do you have rights?") unless sys("mv \"#{VISOR_BUNDLE}\" \"#{SIMBL_PLUGINS_DIR}\"")
    puts blue("Done!")+" "+red("Restart Terminal.app")
  end
end

task :default => :release
