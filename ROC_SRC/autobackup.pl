#!/usr/bin/env perl

$BACKUPDIR = "/envy/stable/backup";
$topdir = " /envy/stable/clan /envy/stable/player ";
$clan = " /envy/area/jewei.are /envy/area/xian.are /envy/area/tina.are /envy/a  rea/slay.are /envy/area/kashoe.are /envy/area/evita.are /envy/area/killua.are   /envy/area/duran.are /envy/area/critical.are /envy/area/zener.are /envy/area/b  ell.are /envy/area/cid.are /envy/area/blaz.are /envy/area/western.are /envy/ar  ea/kittens.are /envy/area/doggie.are /envy/area/vashiwa.are /envy/area/sainte.  are /envy/area/say.are ";
$stamp = time;
$now = localtime;
@lnow = localtime;
$year = $lnow[5] + 1900;
$month = $lnow[4] + 1;
$day = $lnow[3];
if( $year < 1900 ) { $year += 1900 ;}
if( $month < 10 ) { $month = "0$month";}
if( $day < 10 ) { $day = "0$day";}

print STDOUT "Starting Backup clan area... \n";
system "tar zcvf $BACKUPDIR/roc_clanarea_$year$month$day.tgz $clan";

print STDOUT "Roc backup script:";
$stamp = "$year$month$day";
$stamp = "$BACKUPDIR/roc_autobackup_$stamp.tar.gz";

print STDOUT "Starting Autobackup at $now" ;
open TAR, "tar zcvf $stamp --exclude [Bb]ackup $topdir |";
while( <TAR> ) { print ;};
