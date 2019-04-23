#declare block_hash=$(bitcoin-cli getblockhash 455943)
declare quote=$'"'
#block_hash=$quote$block_hash$quote
#echo $block_hash

declare start=$1
declare end=$2

declare dir="/mnt/wd2/blockchain/json1"

for (( i=$start; i <= $end; i++ ))
do
    echo $i
    declare block_hash=`bitcoin-cli getblockhash $i`
    declare get_block_command=$"bitcoin-cli getblock "$block_hash" 2"
    declare block_info=$(eval $get_block_command)
    declare block_height=$(echo "$block_info" | jq ".height")
    echo "$block_height"
    echo "$block_info">>"$dir/b$i.json"
    block_hash=$(echo "$block_info" | jq ".previousblockhash")
    echo $block_hash
done
