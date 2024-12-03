# # import psycopg2
# # import csv

# # # 连接到 PostgreSQL 数据库
# # conn = psycopg2.connect(
# #     dbname="pyfides",    # 替换为你的数据库名称
# #     user="py",      # 替换为你的用户名
# #     password="shuai1016!",  # 替换为你的密码
# #     host="localhost",          # 替换为你的主机名或IP地址
# #     port="5432"                # 替换为你的端口，默认是5432
# # )

# # cur = conn.cursor()

# # # 打开 CSV 文件并读取数据
# # with open('/opt/og/openGauss-server/sse_data/athletes.csv', 'r') as f:
# #     reader = csv.DictReader(f)
# #     i=0
# #     for row in reader:
# #         i=i+1
# #         # 处理空值为 None
# #         row = {key: value if value != '' else None for key, value in row.items()}
        
# #         # 插入数据到表中
# #         cur.execute(
# #             """
# #             Fides_insert INTO athletes(ind, name, short_name) #没法这么执行
# #             VALUES (%s, %s, %s)
# #             """,
# #             (
# #                 i,
# #                 row['name'], 
# #                 row['short_name']
# #             )
# #         )

# # # 提交事务并关闭连接
# # conn.commit()
# # cur.close()
# # conn.close()

# import csv
# import subprocess

# # CSV 文件路径
# csv_file_path = '/opt/og/openGauss-server/sse_data/athletes.csv'

# # SQL 文件路径
# sql_file_path = '/opt/og/openGauss-server/sse_sql/insert_commands.sql'

# # 初始化计数器
# ind = 1

# # 打开 CSV 文件并读取数据
# with open(csv_file_path, 'r') as csv_file:
#     csv_reader = csv.reader(csv_file)
    
#     # 跳过标题行
#     headers = next(csv_reader)
    
#     # 生成 SQL 插入语句
#     insert_statements = []
#     for row in csv_reader:
#         # 取前两个数据
#         col1, col2 = row[:2]
        
#         # 生成 SQL 插入语句，包含计数器 ind
#         insert_statements.append(
#             f"fides_INSERT INTO athletes VALUES ('{ind}', '{col1}', '{col2}');"
#         )
        
#         # 自增计数器
#         ind += 1

# # 将 SQL 插入语句写入文件
# with open(sql_file_path, 'w') as sql_file:
#     sql_file.write('\n'.join(insert_statements))

# # 使用 gsql 执行 SQL 文件
# subprocess.run([
#     'gsql',
#     '-d', 'testfides',
#     '-p', '5432',
#     '-f', sql_file_path
# ])











import csv
import subprocess

# # CSV 文件路径
# csv_file_path = '/opt/og/openGauss-server/sse_data/athletes.csv'

# # SQL 文件路径
sql_file_path = '/opt/og/openGauss-server/sse_sql/test_1-1000-2.sql'

# # 初始化计数器
# ind = 1
# batch_size = 1000  # 每个 INSERT 语句中包含的行数
# values = []  # 用于存储所有 VALUES

# # 打开 CSV 文件并读取数据
# with open(csv_file_path, 'r') as csv_file:
#     csv_reader = csv.reader(csv_file)
    
#     # 跳过标题行
#     headers = next(csv_reader)
    
#     # 生成 SQL 插入语句
#     for row in csv_reader:
#         # 取前两个数据
#         col1, col2 = row[:2]
        
#         # 累积 VALUES
#         values.append(f"('{ind}', '{col1}', '{col2}')")
        
#         # 当累积到 batch_size 行时，生成一次 SQL 语句
#         if len(values) == batch_size:
#             insert_statement = f"fides_INSERT INTO athletes VALUES {','.join(values)};"
#             with open(sql_file_path, 'a') as sql_file:
#                 sql_file.write(insert_statement + '\n')
#             values = []  # 清空列表以便继续累积

#         # 自增计数器
#         ind += 1

#     # 如果剩余的 VALUES 不足 batch_size 行，也生成 SQL 语句
#     if values:
#         insert_statement = f"fides_INSERT INTO athletes VALUES {','.join(values)};"
#         with open(sql_file_path, 'a') as sql_file:
#             sql_file.write(insert_statement + '\n')

# # 使用 gsql 执行 SQL 文件
subprocess.run([
    'gsql',
    '-d', 'testfides',
    '-p', '5432',
    '-f', sql_file_path
])