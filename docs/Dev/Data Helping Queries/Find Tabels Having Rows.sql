SELECT OBJECT_NAME(OBJECT_ID) TableName, st.row_count
FROM sys.dm_db_partition_stats st
WHERE index_id < 2  
and OBJECT_NAME(OBJECT_ID) not in (
 'constants', 'organizations', 'membership', 'roles' , 'status', 'Status_States'
 , 'shelves', 'title_status', 'titles'
 , 'users', 'users_titles' )
 and OBJECT_NAME(OBJECT_ID) not like 'sys%'
 and OBJECT_NAME(OBJECT_ID) not like '%.bak'
 and OBJECT_NAME(OBJECT_ID) not like 'queue%' 
 and st.row_count > 0
 ORDER BY TableName 