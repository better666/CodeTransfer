CodeTransfer
===========

email : ylmbtm@163.com
QQ    : 191342599

介绍
一个轻量级的protobuf的替换方案.
把一个定义的消息格式, 转换为不同语言的包协议解析代码, 目前支持四种语言, c++, go,  lua, c#语言.
使用方便,不需要引第三方库.


导出c#
CodeTransfer -i[InDir] -[OutDir] -tcs

导出go
CodeTransfer -i[InDir] -[OutDir] -tgo

导出c++
CodeTransfer -i[InDir] -[OutDir] -tcpp


导出lua
CodeTransfer -i[InDir] -[OutDir] -tlua

导出全部格式
CodeTransfer -i[InDir] -[OutDir] -tall

