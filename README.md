# my_qt_code

lisb,

## db-viewer

mysql GUI 客户端， 就是dbeaver那种，不过很简单，只需要：

需求
  - 上面操作框，下面显示数据信息
  - 操作框里：直接就一个 框，让人输入 sql语句， 要记录下这些sql。 和 dbeaver一样，就是一个 文本编辑框， 选中几行sql，然后 ctrl+return 执行(没有快捷键)。
  - 下半部分 也是 和 dbeaver一样，每条sql的结果 都新建一个 标签页。
  - 操作框的 再上面一点，弄一个 下拉框， 显示 可以连接的 数据库， 这些 数据库的信息 直接明文 保存到 一个单独的文本中，这样，每次启动的时候，读取这个文本，就知道 可以连那些了。


## list-file

文件查看器

需求
  - 类似 everything 的那种显示，就是 path(从根目录开始到文件名)，size。
  - 可以多选(ctrl+点击，或勾选)，然后选择删除/打开。
  - 可以打开，就是用合适的app打开这些文件，比如 vscode打开 txt文件， vlc打开视频，图片。另起一个窗口的打开(等于是 新进程)， 不是 qt内部打开。 或者有个 预览？
  - 可以自己选择 显示哪些文件。 弄一个输入框，输入 目录， 只展现这个目录中的所有文件。



