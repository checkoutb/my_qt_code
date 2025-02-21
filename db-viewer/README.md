
sql:
- 不要手动换行，QString.replace无法替换掉 \n,\r 。不知道原因， 转成std::string后, qDebug打印出来是 \xE2\x80\xA9
- 只执行选中的sql
