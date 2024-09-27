需要先配置环境：
1、配置变量QTDIR到msvc一级，例：D:\Qt\5.6.3\5.6.3\msvc2015_64
2、确保变量Path包含了到bin一级，例：D:\Qt\5.6.3\5.6.3\msvc2015_64\bin

确保cmake的版本号不低于3.15

生成的sln在../Build/{platform}_{config}/project中