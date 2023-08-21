[ -n "$PROGRAM"    ] || PROGRAM=`basename $PWD`
[ -n "$BUILD_DIR"  ] || BUILD_DIR=.build
[ -n "$BUILD_TYPE" ] || BUILD_TYPE=Release
[ -n "$OBJECT"     ] || OBJECT="${BUILD_DIR}/${PROGRAM}/${PROGRAM}.uf2"
if [ `uname` = Darwin ] ; 
then 
  [ -n "$MOUNTPOINT" ] || MOUNTPOINT=/Volumes/RPI-RP2
  [ -n "$PORT"       ] || PORT=`ls -1t /dev/cu.usb* | head -1`
else
  [ -n "$MOUNTPOINT" ] || MOUNTPOINT=/media/$USER/RPI-RP2
  [ -n "$PORT"       ] || PORT=/dev/ttyACM0
fi
[ -n "$BAUD"       ] || BAUD=115200

function die {
  echo "ERROR: $1"
  exit ${2:-1}
}

function waitfor {
  item=$1
  limit=${2:-5}
  while [ \! -e "$item" ] ; do
      [ $limit -gt 0 ] || die "   $item not ready"
      echo -n "Waiting for $item ($limit)..."
      sleep 1
      echo ""
      limit=`expr $limit - 1`
  done
}
