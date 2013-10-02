#!/usr/bin/perl
#
# Program to do the obvious
#
$top_100_list="/var/www/test/top100.html";
$merge_file="/var/upload/Merged.score";

$failed_file="/var/upload/failed_file";
$upload_file_begin="/var/upload/upload";
$upload_dir="/var/upload";

$html_begin_file="/var/upload/top100begin.html";
$html_end_file="/var/upload/top100end.html";


system("chmod -R a+r $upload_dir");

for($x=0 ; $x <1000; ++$x) {
    $file = "$upload_file_begin$x";

    if(open(FILE,$file)) {
    $mihail_return = system("/usr/local/bin/mihail $merge_file $file $merge_file > /dev/null") >> 8;

    if($mihail_return == 2) {
	    system("cp $file $failed_file$x");
	}
    } 
}

system("cp $html_begin_file $top_100_list");
system("/usr/local/bin/mihail -h $merge_file >> $top_100_list");
system("cat $html_end_file >> $top_100_list");

