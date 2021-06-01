# 南开大学2020fall 编译原理大作业

## 任务阐述：

以给定的[SysY语言](https://gitlab.eduxiji.net/nscscc/compiler2021/-/blob/master/SysY%E8%AF%AD%E8%A8%80%E5%AE%9A%E4%B9%89.pdf)为基础文法，在基础文法上添加for/struct等C语言特性，实现一个简单的编译器

借助flex和yacc工具进行词法分析和语法分析，构建抽象语法树，再在AST上生成以AT&T x86为目标语言的汇编代码。编译器的构建主要应用龙书前六章知识，注重于前端的训练。

## 后续：

目前已移植到目标语言为arm平台，可编译语言特性仅包括SysY文法。由于目前正在参与[第二届编译系统设计赛](https://course.educg.net/sv2/indexexp/contest/contest.jsp?contestID=3SpdT9reSjk)，因此该版本项目暂不公开。

目标为借助AST生成三地址码，再由三地址码转换到SSA，再以SSA为IR进行后端优化，以提高编译后的运行效率。
