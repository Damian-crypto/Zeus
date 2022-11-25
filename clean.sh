RED='\033[0;31m';
GREEN='\033[0;32m';
NONE='\033[0;0m';

cd build;
printf "${RED}Deleting CMakeCache.txt${NONE}\n";
rm -rf CMakeCache.txt;
printf "${RED}Deleting Sandbox${NONE}\n";
rm -rf Sandbox;
printf "${RED}Deleting SimpleGame${NONE}\n";
rm -rf SimpleGame
printf "${GREEN}Local cleanup completed!${NONE}\n";
