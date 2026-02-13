import random
from datetime import date, timedelta

def get_random_date(start_date:date = date(1949, 10, 1), end_date: date = date.today()) -> date:
    time_between = end_date - start_date
    days_between = time_between.days
    random_number_of_days = random.randrange(days_between)
    return start_date + timedelta(days=random_number_of_days)

def get_random_date_str(start_date:date = date(1949, 10, 1), end_date: date = date.today()) -> str:
    ret_date = get_random_date(start_date, end_date)
    return "{:04d}{:04d}{:04d}".format(ret_date.year, ret_date.month, ret_date.day)

def get_sddf

def get_random_id() :
    r_code = ["1", "0", "X", "9", "8","7","6","5","4","3","2"]
    pos_code = "110000" # Beijing

    date_str = get_random_date_str()
    rnum = random.randint(0, 999)
    ret_s = pos_code + date_str + "{:03d}".format(rnum)
    