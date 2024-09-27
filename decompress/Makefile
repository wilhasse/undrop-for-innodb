aall: mysql decompress

decompress: decompress.c
	libs=`find .lib/`
	#gcc -Iinclude -Iinclude/zlib -lpthread -lz -o decompress decompress.c .lib/*
	#g++ -DUNIV_DEBUG -DUNIV_ZIP_DEBUG -Iinclude -Iinclude/zlib -lpthread -lz -lm -ldl -lcrypt -lrt -lncurses 
	g++ \
	       	-DHAVE_CONFIG_H -DHAVE_IB_GCC_ATOMIC_BUILTINS=1 -DHAVE_IB_ATOMIC_PTHREAD_T_GCC=1 -DSIZEOF_PTHREAD_T=8 -Wall -g -DENABLED_DEBUG_SYNC -DSAFE_MUTEX -DUNIV_DEBUG  -DUNIV_LINUX -D_GNU_SOURCE=1 \
                -laio \
		-Iinclude -Iinclude/zlib \
		-I ./mysql-server-mysql-5.5.16/include/ \
		-L/usr/lib/ -L/usr/lib64/ \
		-lpthread -lz -lm -ldl -lcrypt -lrt -lncurses \
		.lib/log0recv.c.o .lib/ut0dbg.c.o .lib/ut0ut.c.o .lib/ha_innodb.cc.o .lib/os0thread.c.o .lib/os0sync.c.o .lib/my_static.c.o .lib/rem0rec.c.o .lib/mem0mem.c.o .lib/trx0sys.c.o \
		.lib/btr0btr.c.o .lib/btr0cur.c.o .lib/btr0sea.c.o .lib/buf0buf.c.o .lib/buf0flu.c.o .lib/buf0lru.c.o .lib/buf0rea.c.o .lib/charset.c.o .lib/ctype-simple.c.o .lib/data0data.c.o .lib/data0type.c.o .lib/dict0dict.c.o .lib/dict0mem.c.o .lib/dyn0dyn.c.o .lib/fil0fil.c.o .lib/fsp0fsp.c.o .lib/hash0hash.c.o .lib/ibuf0ibuf.c.o .lib/int2str.c.o .lib/libmysql.c.o .lib/lock0lock.c.o .lib/log0log.c.o .lib/log.cc.o .lib/mem0pool.c.o .lib/mf_dirname.c.o .lib/mf_pack.c.o .lib/mtr0log.c.o .lib/mtr0mtr.c.o .lib/mulalloc.c.o .lib/my_alloc.c.o .lib/my_error.c.o .lib/my_malloc.c.o .lib/my_mess.c.o .lib/my_open.c.o .lib/my_thr_init.c.o .lib/os0file.c.o .lib/os0proc.c.o .lib/page0cur.c.o .lib/page0page.c.o .lib/read0read.c.o .lib/row0merge.c.o .lib/row0mysql.c.o .lib/row0sel.c.o .lib/sql_cache.cc.o .lib/sql_class.cc.o .lib/srv0srv.c.o .lib/srv0start.c.o .lib/strend.c.o .lib/sync0arr.c.o .lib/sync0rw.c.o .lib/sync0sync.c.o .lib/trx0i_s.c.o .lib/trx0purge.c.o .lib/trx0rec.c.o .lib/trx0roll.c.o .lib/trx0rseg.c.o .lib/trx0trx.c.o .lib/trx0undo.c.o .lib/usr0sess.c.o .lib/ut0bh.c.o .lib/ut0mem.c.o .lib/row0upd.c.o \
		.lib/array.c.o .lib/bchange.c.o .lib/bmove_upp.c.o .lib/btr0pcur.c.o .lib/buf0buddy.c.o .lib/charset-def.c.o .lib/client.c.o .lib/client_plugin.c.o .lib/ctype-bin.c.o .lib/ctype.c.o .lib/decimal.c.o .lib/dict0boot.c.o .lib/dict0crea.c.o .lib/dict0load.c.o .lib/dtoa.c.o .lib/errmsg.c.o .lib/errors.c.o .lib/eval0eval.c.o .lib/fut0lst.c.o .lib/ha0ha.c.o .lib/ha0storage.c.o .lib/handler0alter.cc.o .lib/handler.cc.o .lib/hash.c.o .lib/is_prefix.c.o .lib/item_func.cc.o .lib/lexyy.c.o .lib/list.c.o .lib/llstr.c.o .lib/lock0iter.c.o .lib/mach0data.c.o .lib/mf_cache.c.o .lib/mf_fn_ext.c.o .lib/mf_format.c.o .lib/mf_iocache2.c.o .lib/mf_iocache.c.o .lib/mf_tempdir.c.o .lib/mf_tempfile.c.o .lib/my_chsize.c.o .lib/my_create.c.o .lib/my_delete.c.o .lib/my_div.c.o .lib/my_fopen.c.o .lib/my_getsystime.c.o .lib/my_getwd.c.o .lib/my_init.c.o .lib/my_lib.c.o .lib/my_mmap.c.o .lib/my_once.c.o .lib/my_pread.c.o .lib/my_pthread.c.o .lib/my_seek.c.o .lib/my_sync.c.o .lib/my_time.c.o .lib/my_vsnprintf.c.o .lib/my_write.c.o .lib/net_serv.cc.o .lib/pack.c.o .lib/pars0pars.c.o .lib/password.c.o .lib/que0que.c.o .lib/rem0cmp.c.o .lib/row0ext.c.o .lib/row0ins.c.o .lib/row0purge.c.o .lib/row0row.c.o .lib/row0undo.c.o .lib/row0vers.c.o .lib/rpl_handler.cc.o .lib/sql_audit.cc.o .lib/sql_state.c.o .lib/strmov.c.o .lib/strnmov.c.o .lib/strxmov.c.o .lib/strxnmov.c.o .lib/thr_alarm.c.o .lib/ut0rbt.c.o .lib/ut0rnd.c.o .lib/ut0vec.c.o .lib/vio.c.o \
		.lib/default.c.o .lib/eval0proc.c.o .lib/mf_keycache.c.o .lib/mi_keycache.c.o .lib/my_bitmap.c.o .lib/my_compress.c.o .lib/my_fstream.c.o .lib/my_symlink2.c.o .lib/my_symlink.c.o .lib/pars0grm.c.o .lib/pars0opt.c.o .lib/pars0sym.c.o .lib/queues.c.o .lib/row0uins.c.o .lib/row0umod.c.o .lib/sha1.c.o .lib/str2int.c.o .lib/strcend.c.o .lib/typelib.c.o .lib/viosocket.c.o .lib/xml.c.o \
		.lib/ha_myisam.cc.o .lib/mf_arr_appstr.c.o .lib/mf_keycaches.c.o .lib/mf_loadpath.c.o .lib/mf_same.c.o .lib/mi_info.c.o .lib/mi_static.c.o .lib/my_largepage.c.o .lib/my_rename.c.o \
		.lib/mysqld.cc.o .lib/sql_error.cc.o .lib/ctype-latin1.c.o .lib/field.cc.o .lib/sql_table.cc.o .lib/sql_list.cc.o .lib/thr_malloc.cc.o .lib/sql_show.cc.o \
		.lib/checksum.c.o .lib/ft_parser.c.o .lib/ft_static.c.o .lib/ft_stopwords.c.o .lib/mf_wcomp.c.o .lib/mi_check.c.o .lib/mi_close.c.o .lib/mi_create.c.o .lib/mi_delete_all.c.o .lib/mi_delete_table.c.o .lib/mi_extra.c.o .lib/mi_locking.c.o .lib/mi_log.c.o .lib/mi_open.c.o .lib/mi_panic.c.o .lib/mi_preload.c.o .lib/mi_range.c.o .lib/mi_rename.c.o .lib/mi_rfirst.c.o .lib/mi_rkey.c.o .lib/mi_rlast.c.o .lib/mi_rnext.c.o .lib/mi_rnext_same.c.o .lib/mi_rprev.c.o .lib/mi_rrnd.c.o .lib/mi_scan.c.o .lib/mi_write.c.o .lib/my_access.c.o .lib/my_file.c.o .lib/my_getopt.c.o .lib/my_sleep.c.o .lib/pfs_server.cc.o .lib/ptr_cmp.c.o .lib/reginit.c.o .lib/repl_failsafe.cc.o .lib/slave.cc.o .lib/sql_base.cc.o .lib/sql_manager.cc.o .lib/sql_parse.cc.o .lib/sql_plugin.cc.o .lib/sql_repl.cc.o .lib/stacktrace.c.o .lib/sys_vars.cc.o .lib/strfunc.cc.o \
		.lib/ctype-utf8.c.o .lib/i_s.cc.o .lib/table.cc.o .lib/my_read.c.o .lib/ctype-uca.c.o .lib/strmake.c.o .lib/my_strtoll10.c.o .lib/log_event.cc.o \
		.lib/mdl.cc.o .lib/sql_time.cc.o .lib/rpl_filter.cc.o .lib/transaction.cc.o .lib/item_strfunc.cc.o .lib/scheduler.cc.o .lib/rpl_record.cc.o \
		.lib/sql_lex.cc.o .lib/lock.cc.o .lib/sql_handler.cc.o .lib/sql_acl.cc.o .lib/item_subselect.cc.o .lib/item.cc.o .lib/protocol.cc.o \
		.lib/sp_rcontext.cc.o .lib/sql_profile.cc.o .lib/sp_cache.cc.o \
		.lib/ctype-big5.c.o .lib/ctype-win1250ch.c.o .lib/ctype-cp932.c.o .lib/ctype-czech.c.o .lib/ctype-eucjpms.c.o .lib/ctype-euc_kr.c.o \
		.lib/ctype-gb2312.c.o .lib/ctype-gbk.c.o .lib/ctype-sjis.c.o .lib/ctype-tis620.c.o .lib/ctype-ucs2.c.o .lib/ctype-ujis.c.o \
		.lib/ctype-extra.c.o \
		.lib/key.cc.o .lib/ha_partition.cc.o .lib/sql_trigger.cc.o .lib/discover.cc.o .lib/my_bit.c.o .lib/sp_head.cc.o .lib/sp.cc.o \
		.lib/set_var.cc.o .lib/my_decimal.cc.o .lib/item_cmpfunc.cc.o .lib/rpl_rli.cc.o .lib/sql_udf.cc.o \
		.lib/thr_rwlock.c.o .lib/sql_signal.cc.o .lib/keycaches.cc.o .lib/rpl_reporting.cc.o .lib/ctype-mb.c.o .lib/mf_qsort2.c.o .lib/mf_qsort.c.o \
		.lib/my_lock.c.o .lib/mi_page.c.o .lib/mi_key.c.o .lib/field_conv.cc.o \
		.lib/ft_boolean_search.c.o  .lib/ft_nlq_search.c.o  .lib/ft_update.c.o  .lib/md5.c.o  .lib/mf_unixpath.c.o  .lib/mi_cache.c.o  .lib/mi_checksum.c.o  .lib/mi_dbug.c.o  .lib/mi_delete.c.o  .lib/mi_dynrec.c.o  .lib/mi_packrec.c.o  .lib/mi_search.c.o  .lib/mi_statrec.c.o  .lib/mi_unique.c.o  .lib/mi_update.c.o  .lib/my_aes.c.o  .lib/my_alarm.c.o  .lib/my_compare.c.o  .lib/my_gethwaddr.c.o  .lib/my_redel.c.o  .lib/my_strchr.c.o  .lib/my_user.c.o  .lib/regcomp.c.o  .lib/regerror.c.o  .lib/regexec.c.o  .lib/regfree.c.o  .lib/rt_index.c.o  .lib/rt_key.c.o  .lib/sort.c.o  .lib/sp_key.c.o  .lib/sql_connect.cc.o  .lib/tap.c.o  .lib/tokenizer.c.o  .lib/tree.c.o \
		.lib/mf_getdate.c.o  .lib/rijndael.c.o  .lib/rt_mbr.c.o  .lib/rt_split.c.o \
		.lib/sql_locale.cc.o .lib/pfs_global.cc.o .lib/pfs.cc.o \
		.lib/parse_file.cc.o .lib/sql_test.cc.o .lib/pfs_events_waits.cc.o .lib/sql_servers.cc.o \
		.lib/common.c.o .lib/datadict.cc.o .lib/derror.cc.o .lib/emacs.c.o .lib/event_data_objects.cc.o .lib/event_db_repository.cc.o .lib/event_parse_data.cc.o .lib/event_queue.cc.o .lib/events.cc.o .lib/event_scheduler.cc.o .lib/filesort.cc.o .lib/gstream.cc.o .lib/ha_heap.cc.o .lib/ha_myisammrg.cc.o .lib/ha_perfschema.cc.o .lib/ha_tina.cc.o .lib/history.c.o .lib/hostname.cc.o .lib/hp_hash.c.o .lib/init.cc.o .lib/item_create.cc.o .lib/item_sum.cc.o .lib/item_timefunc.cc.o .lib/item_xmlfunc.cc.o .lib/log_event_old.cc.o .lib/map.c.o .lib/my_atomic.c.o .lib/opt_range.cc.o .lib/partition_info.cc.o .lib/pfs_atomic.cc.o .lib/pfs_check.cc.o .lib/pfs_engine_table.cc.o .lib/pfs_instr.cc.o .lib/pfs_instr_class.cc.o .lib/pfs_timer.cc.o .lib/readline.c.o .lib/records.cc.o .lib/refresh.c.o .lib/rpl_injector.cc.o .lib/rpl_mi.cc.o .lib/semisync_slave.cc.o .lib/sql_admin.cc.o .lib/sql_binlog.cc.o .lib/sql_cursor.cc.o .lib/sql_db.cc.o .lib/sql_delete.cc.o .lib/sql_derived.cc.o .lib/sql_do.cc.o .lib/sql_help.cc.o .lib/sql_insert.cc.o .lib/sql_load.cc.o .lib/sql_partition.cc.o .lib/sql_prepare.cc.o .lib/sql_reload.cc.o .lib/sql_rename.cc.o .lib/sql_select.cc.o .lib/sql_tablespace.cc.o .lib/sql_update.cc.o .lib/sql_view.cc.o .lib/sql_yacc.cc.o .lib/table_events_waits.cc.o .lib/term.c.o .lib/uniques.cc.o .lib/unireg.cc.o .lib/vi.c.o \
		.lib/base64.c.o .lib/chared.c.o .lib/el.c.o .lib/fcns.c.o .lib/fgetln.c.o .lib/filecomplete.c.o .lib/help.c.o .lib/hist.c.o .lib/hp_block.c.o .lib/hp_clear.c.o .lib/hp_close.c.o .lib/hp_create.c.o .lib/hp_delete.c.o .lib/hp_extra.c.o .lib/hp_info.c.o .lib/hp_open.c.o .lib/hp_panic.c.o .lib/hp_rename.c.o .lib/hp_rfirst.c.o .lib/hp_rkey.c.o .lib/hp_rlast.c.o .lib/hp_rnext.c.o .lib/hp_rprev.c.o .lib/hp_rrnd.c.o .lib/hp_scan.c.o .lib/hp_static.c.o .lib/hp_update.c.o .lib/hp_write.c.o .lib/item_buff.cc.o .lib/key.c.o .lib/lf_alloc-pin.c.o .lib/lf_hash.c.o .lib/mf_iocache.cc.o .lib/mf_sort.c.o .lib/my_mkdir.c.o .lib/my_rdtsc.c.o .lib/myrg_close.c.o .lib/myrg_create.c.o .lib/myrg_delete.c.o .lib/myrg_extra.c.o .lib/myrg_info.c.o .lib/myrg_locking.c.o .lib/myrg_open.c.o .lib/myrg_panic.c.o .lib/myrg_range.c.o .lib/myrg_records.c.o .lib/myrg_rfirst.c.o .lib/myrg_rkey.c.o .lib/myrg_rlast.c.o .lib/myrg_rnext.c.o .lib/myrg_rnext_same.c.o .lib/myrg_rprev.c.o .lib/myrg_rrnd.c.o .lib/myrg_static.c.o .lib/myrg_update.c.o .lib/myrg_write.c.o .lib/opt_sum.cc.o .lib/parse.c.o .lib/pfs_column_values.cc.o .lib/procedure.cc.o .lib/prompt.c.o .lib/read.c.o .lib/rpl_record_old.cc.o .lib/search.c.o .lib/sp_pcontext.cc.o .lib/sql_analyse.cc.o .lib/sql_builtin.cc.o .lib/sql_union.cc.o .lib/str_alloc.c.o .lib/strfill.c.o .lib/strlcat.c.o .lib/strlcpy.c.o .lib/tty.c.o .lib/unvis.c.o .lib/vis.c.o \
		.lib/lf_dynarray.c.o .lib/mf_radix.c.o .lib/myrg_queue.c.o .lib/_rectest.c.o .lib/sig.c.o .lib/tztime.cc.o \
		.lib/rpl_tblmap.cc.o .lib/rpl_utility.cc.o .lib/spatial.cc.o .lib/item_row.cc.o .lib/transparent_file.cc.o \
		.lib/sql_crypt.cc.o .lib/item_geofunc.cc.o .lib/table_setup_consumers.cc.o .lib/table_setup_instruments.cc.o \
		.lib/table_setup_timers.cc.o .lib/table_performance_timers.cc.o .lib/table_threads.cc.o .lib/table_events_waits_summary.cc.o  \
		.lib/table_ews_global_by_event_name.cc.o .lib/table_file_summary.cc.o \
		.lib/table_sync_instances.cc.o .lib/table_file_instances.cc.o .lib/semisync.cc.o \
		.lib/table_all_instr.cc.o .lib/sql_partition_admin.cc.o .lib/sql_alter.cc.o .lib/sql_truncate.cc.o \
		.lib/page0zip.c.o \
		.lib/dbug.c.o .lib/debug_sync.cc.o .lib/thr_mutex.c.o .lib/sql_string.cc.o \
		.lib/thr_lock.c.o .lib/_check.c.o \
		-o decompress decompress.c

page0zip.o:
	gcc -c -Iinclude -Iinclude/zlib page0zip.c 

mysql:
#	if ! test -d mysql-server-mysql-5.5.16 ; then wget http://www.percona.com/redir/downloads/community/mysql-server-mysql-5.5.16.tar.gz \
#		tar zxf mysql-server-mysql-5.5.16.tar.gz ; fi 
	cd mysql-server-mysql-5.5.16 && cmake . -DWITH_DEBUG=1 && make && cd -
	mkdir -p .lib
	find mysql-server-mysql-5.5.16 -name "*.o" -exec cp '{}' .lib/ \;
	cp mysql-server-mysql-5.5.16/sql/CMakeFiles/sql.dir/tztime.cc.o .lib/
	cp mysql-server-mysql-5.5.16/mysys/CMakeFiles/mysys.dir/thr_lock.c.o .lib/
	cp mysql-server-mysql-5.5.16/sql/CMakeFiles/sql.dir/sql_string.cc.o .lib
	
clean:
	rm -fr decompress .lib
