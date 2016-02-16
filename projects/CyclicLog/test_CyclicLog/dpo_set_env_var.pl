#!/usr/bin/perl -w

use strict;
use Cwd;

my $cwd = getcwd();

my $scripts_dir = $ENV{DPO2_CORE_ROOT} . "/scripts";

chdir($scripts_dir);

system("$scripts_dir/dpo_set_env_var_impl.pl --source \"$cwd\"");

chdir($cwd);

