import os
from collections import defaultdict

# 定义要统计的文件扩展名
CODE_FILE_EXTENSIONS = {
    '.java', '.xml', '.py', '.cpp', '.h', '.c', '.hpp',
    '.js', '.html', '.css', '.php', '.sh', '.go',
    '.md', '.txt', '.json', '.yaml', '.yml', '.sql'
}

def is_code_file(filename):
    """检查文件是否是代码文件"""
    return os.path.splitext(filename)[1].lower() in CODE_FILE_EXTENSIONS

def is_empty_line(line):
    """检查是否是空行（无内容或只有空格/换行符）"""
    return not line.strip()

def count_file_lines(filepath):
    """统计单个文件的行数（包括空行和非空行）"""
    total_lines = 0
    empty_lines = 0
    
    try:
        with open(filepath, 'r', encoding='utf-8') as file:
            for line in file:
                total_lines += 1
                if is_empty_line(line):
                    empty_lines += 1
    except UnicodeDecodeError:
        # 跳过二进制文件
        return None
    except Exception as e:
        print(f"Error reading {filepath}: {e}")
        return None
    
    return {
        'total': total_lines,
        'empty': empty_lines,
        'code': total_lines - empty_lines,
        'path': filepath
    }

def scan_directory(directory):
    """扫描目录并统计所有代码文件"""
    stats = defaultdict(lambda: {'files': 0, 'total': 0, 'empty': 0, 'code': 0})
    file_details = []
    
    for root, _, files in os.walk(directory):
        for filename in files:
            if is_code_file(filename):
                filepath = os.path.join(root, filename)
                ext = os.path.splitext(filename)[1].lower()
                line_stats = count_file_lines(filepath)
                
                if line_stats:
                    stats[ext]['files'] += 1
                    stats[ext]['total'] += line_stats['total']
                    stats[ext]['empty'] += line_stats['empty']
                    stats[ext]['code'] += line_stats['code']
                    file_details.append(line_stats)
    
    return stats, file_details

def print_summary(stats):
    """打印统计摘要"""
    print("\n代码文件行数统计:")
    print("-" * 75)
    print(f"{'扩展名':<7}{'文件数':<7}{'总行数':<12}{'空行数':<12}{'代码行数':<11}") # 中文字符占2个英文字符
    print("-" * 75)
    
    total_files = 0
    total_lines = 0
    total_empty = 0
    total_code = 0
    
    for ext, data in stats.items():
        print(f"{ext:<10}{data['files']:<10}{data['total']:<15}{data['empty']:<15}{data['code']:<15}")
        total_files += data['files']
        total_lines += data['total']
        total_empty += data['empty']
        total_code += data['code']
    
    print("-" * 75)
    print(f"{'总计':<8}{total_files:<10}{total_lines:<15}{total_empty:<15}{total_code:<15}")
    print("-" * 75)

def print_file_details(file_details, max_files=10):
    """打印文件详情（默认最多显示10个文件）"""
    print("\n文件详情（按代码行数降序）:")
    print("-" * 120)
    print(f"{'文件路径':<80}{'总行数':<10}{'空行数':<10}{'代码行数':<10}")
    print("-" * 120)
    
    for detail in sorted(file_details, key=lambda x: -x['code'])[:max_files]:
        print(f"{detail['path'][:80]:<80}{detail['total']:<10}{detail['empty']:<10}{detail['code']:<10}")
    
    print("-" * 120)

if __name__ == '__main__':
    directory = './EllipCurve'
    
    if not os.path.exists(directory):
        print(f"错误: 目录 '{directory}' 不存在")
        exit()
    
    print(f"扫描目录: {directory}")
    stats, file_details = scan_directory(directory)
    
    print_summary(stats)
    # print_file_details(file_details)
