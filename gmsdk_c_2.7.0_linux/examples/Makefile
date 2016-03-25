all:
	g++ test_md_live.cpp -lpthread -o test_md_live -I../inc -L../lib -lgm
	g++ test_md_query.cpp -o test_md_query -I../inc -L../lib -lgm 
	g++ test_md_playback.cpp -o test_md_playback -I../inc -L../lib/ -lgm
	g++ test_td_live.cpp -o test_td_live -I../inc -L../lib -lgm
	g++ test_strategy.cpp -o test_strategy -I../inc -L../lib -lgm

test_md_live:
	g++ -pthread -g test_md_live.cpp -o test_md_live -I../inc -L../lib -lgm
test_md_query:
	g++ test_md_query.cpp -o test_md_query -I../inc -L../lib -lgm
test_md_playback:
	g++ test_md_playback_online.cpp -o test_md_playback -I../inc -L../lib -lgm
test_tr_live:
	g++ test_td_live.cpp -o test_td_live -I../inc -L../lib -lgm
test_strategy:
	g++ test_strategy.cpp -o test_strategy -I../inc -L../lib -lgm

clean:
	rm test_md_live test_md_query test_md_playback test_tr_live test_strategy test_td_live
