## 观察预处理结果
注意到输出中有很多
```
# 0 "a.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "a.c"

# 1 "/usr/include/stdio.h" 1 3 4
# 28 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/bits/libc-header-start.h" 1 3 4
# 33 "/usr/include/bits/libc-header-start.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 415 "/usr/include/features.h" 3 4
# 1 "/usr/include/features-time64.h" 1 3 4
```
这种语句，以第一行为例，表明此时正在处理`a.c`的第零行，可以从这种注释分析预处理的内容。
1. 前一部分来自`/usr/include/stdio.h`，这是被`#include`进来的文件，值得注意的是，该文件中的`#include`也会被递归引入，比如文件的第一行正式内容来自于`stddef.h`；
2. 紧接着处理了三条`#define`，在预处理文件中变成了空白行；
3. 接着处理到第8行`main`函数
4. `MSG`被替换成了定义值，但是没有换行，`define`中的`\`被自动连接了；
5. 注释被删除了；
6. `ifdef`没有match，该部分被替换成了空白行；
7. `_concat`和`_str`被执行，`#x`代表把x转换为字符串，`a##b`表示连接A和B，注意，并不会转换为字符串。
## 如何寻找头文件
执行`gcc -E a.c --verbose > /dev/null`
```
#include <...> search starts here:
 /usr/lib/gcc/x86_64-pc-linux-gnu/15.2.1/include
 /usr/local/include
 /usr/lib/gcc/x86_64-pc-linux-gnu/15.2.1/include-fixed
 /usr/include
```
看到预处理中打印的`stdio.h`就存在`/usr/include`中；
`man gcc`中可以看到对于`#include""`会先搜索当前目录，再搜索`-I`指定目录，再搜索标准系统头文件目录；
`-isystem`常用来导入第三方库，在`-I`后、标准库前；
`-idirafter`在标准库后搜索；
相同层级按从左到右从先到后顺序。
使用`gcc -I . -E a.c`包含了`./stdio.h`
## 观察预处理结果(2)
换用`riscv-gcc`后，首先库文件搜索路径变了，然后`ifdef _riscv`命中，生成了语句。
新的搜索路径为
```
#include <...> search starts here:
 .
 /usr/lib/gcc/riscv64-linux-gnu/15.1.0/include
 /usr/lib/gcc/riscv64-linux-gnu/15.1.0/include-fixed
 /usr/lib/gcc/riscv64-linux-gnu/15.1.0/../../../../riscv64-linux-gnu/include
 /usr/riscv64-linux-gnu/usr/include
```
## 对比gcc和riscv64-linux-gnu-gcc的预定义宏
```
1,2d0 这部分是表明是64位x86架构
< #define __amd64 1
< #define __amd64__ 1
6,7d3
< #define __ATOMIC_HLE_ACQUIRE 65536
< #define __ATOMIC_HLE_RELEASE 131072
28d23
< #define __BITINT_MAXWIDTH__ 65535
34c29
< #define __code_model_small__ 1
--- RV64中规定CHAR是无符号整型
> #define __CHAR_UNSIGNED__ 1
51,78d45
< #define __DEC128_EPSILON__ 1E-33DL
< #define __DEC128_MANT_DIG__ 34
< #define __DEC128_MAX__ 9.999999999999999999999999999999999E6144DL
< #define __DEC128_MAX_EXP__ 6145
< #define __DEC128_MIN__ 1E-6143DL
< #define __DEC128_MIN_EXP__ (-6142)
< #define __DEC128_SUBNORMAL_MIN__ 0.000000000000000000000000000000001E-6143DL
< #define __DEC32_EPSILON__ 1E-6DF
< #define __DEC32_MANT_DIG__ 7
< #define __DEC32_MAX__ 9.999999E96DF
< #define __DEC32_MAX_EXP__ 97
< #define __DEC32_MIN__ 1E-95DF
< #define __DEC32_MIN_EXP__ (-94)
< #define __DEC32_SUBNORMAL_MIN__ 0.000001E-95DF
< #define __DEC64_EPSILON__ 1E-15DD
< #define __DEC64_MANT_DIG__ 16
< #define __DEC64_MAX__ 9.999999999999999E384DD
< #define __DEC64_MAX_EXP__ 385
< #define __DEC64_MIN__ 1E-383DD
< #define __DEC64_MIN_EXP__ (-382)
< #define __DEC64_SUBNORMAL_MIN__ 0.000000000000001E-383DD
< #define __DEC64X_EPSILON__ 1E-33D64x
< #define __DEC64X_MANT_DIG__ 34
< #define __DEC64X_MAX__ 9.999999999999999999999999999999999E6144D64x
< #define __DEC64X_MAX_EXP__ 6145
< #define __DEC64X_MIN__ 1E-6143D64x
< #define __DEC64X_MIN_EXP__ (-6142)
< #define __DEC64X_SUBNORMAL_MIN__ 0.000000000000000000000000000000001E-6143D64x
80,81c47
< #define __DECIMAL_BID_FORMAT__ 1
< #define __DECIMAL_DIG__ 21
---
> #define __DECIMAL_DIG__ 36
165,168c131,134
< #define __FLT64X_DECIMAL_DIG__ 21
< #define __FLT64X_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951F64x
< #define __FLT64X_DIG__ 18
< #define __FLT64X_EPSILON__ 1.08420217248550443400745280086994171e-19F64x
---
> #define __FLT64X_DECIMAL_DIG__ 36
> #define __FLT64X_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966F64x
> #define __FLT64X_DIG__ 33
> #define __FLT64X_EPSILON__ 1.92592994438723585305597794258492732e-34F64x
173c139
< #define __FLT64X_MANT_DIG__ 64
---
> #define __FLT64X_MANT_DIG__ 113
175c141
< #define __FLT64X_MAX__ 1.18973149535723176502126385303097021e+4932F64x
---
> #define __FLT64X_MAX__ 1.18973149535723176508575932662800702e+4932F64x
180c146
< #define __FLT64X_NORM_MAX__ 1.18973149535723176502126385303097021e+4932F64x
---
> #define __FLT64X_NORM_MAX__ 1.18973149535723176508575932662800702e+4932F64x
200,201c166,170
< #define __FXSR__ 1
< #define __GCC_ASM_FLAG_OUTPUTS__ 1
---
> #define __FP_FAST_FMA 1
> #define __FP_FAST_FMAF 1
> #define __FP_FAST_FMAF32 1
> #define __FP_FAST_FMAF32x 1
> #define __FP_FAST_FMAF64 1
214,215d182
< #define __GCC_CONSTRUCTIVE_SIZE 64
< #define __GCC_DESTRUCTIVE_SIZE 64
225,226c192,193
< #define __GNUC_MINOR__ 2
< #define __GNUC_PATCHLEVEL__ 1
---
> #define __GNUC_MINOR__ 1
> #define __GNUC_PATCHLEVEL__ 0
231d197
< #define __HAVE_SPECULATION_SAFE_VALUE 1
277,282c243,246
< #define __k8 1
< #define __k8__ 1
< #define __LDBL_DECIMAL_DIG__ 21
< #define __LDBL_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951L
< #define __LDBL_DIG__ 18
< #define __LDBL_EPSILON__ 1.08420217248550443400745280086994171e-19L
---
> #define __LDBL_DECIMAL_DIG__ 36
> #define __LDBL_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966L
> #define __LDBL_DIG__ 33
> #define __LDBL_EPSILON__ 1.92592994438723585305597794258492732e-34L
287c251
< #define __LDBL_MANT_DIG__ 64
---
> #define __LDBL_MANT_DIG__ 113
289c253
< #define __LDBL_MAX__ 1.18973149535723176502126385303097021e+4932L
---
> #define __LDBL_MAX__ 1.18973149535723176508575932662800702e+4932L
294c258
< #define __LDBL_NORM_MAX__ 1.18973149535723176502126385303097021e+4932L
---
> #define __LDBL_NORM_MAX__ 1.18973149535723176508575932662800702e+4932L
304,305d267
< #define __MMX__ 1
< #define __MMX_WITH_SSE__ 1
318a281,307 rv相关，指定扩展
> #define __riscv 1
> #define __riscv_a 2001000
> #define __riscv_arch_test 1
> #define __riscv_atomic 1
> #define __riscv_c 2000000
> #define __riscv_cmodel_medany 1
> #define __riscv_compressed 1
> #define __riscv_d 2002000
> #define __riscv_div 1
> #define __riscv_f 2002000
> #define __riscv_fdiv 1
> #define __riscv_flen 64
> #define __riscv_float_abi_double 1
> #define __riscv_fsqrt 1
> #define __riscv_i 2001000
> #define __riscv_m 2000000
> #define __riscv_misaligned_slow 1
> #define __riscv_mul 1
> #define __riscv_muldiv 1
> #define __riscv_xlen 64
> #define __riscv_zaamo 1000000
> #define __riscv_zalrsc 1000000
> #define __riscv_zca 1000000
> #define __riscv_zcd 1000000
> #define __riscv_zicsr 2000000
> #define __riscv_zifencei 2000000
> #define __riscv_zmmul 1000000
321,322d309
< #define __SEG_FS 1
< #define __SEG_GS 1
331d317
< #define __SIZEOF_FLOAT128__ 16
333d318
< #define __SIZEOF_FLOAT80__ 16
347,351d331
< #define __SSE__ 1
< #define __SSE2__ 1
< #define __SSE2_MATH__ 1
< #define __SSE_MATH__ 1
< #define __SSP_STRONG__ 3
403c383
< #define __VERSION__ "15.2.1 20250813"
---
> #define __VERSION__ "15.1.0"
412,413d391
< #define __x86_64 1
< #define __x86_64__ 1

```
## 了解编译的过程
从Parsing and Semantic Analysis到Assembler应该属于编译。
* 解析文件生成一个抽象语法树（Abstract Syntax Tree or AST）
* 把AST转换为汇编语言assembly，通用是`.s`，嵌入式中的`.asm`应该也是？
* 汇编转机器码
## 常见优化
这个比较有意思，记录一下
1. 常量传播，常量计算结果由常量代替；
2. 死代码消除，溢出不运行的代码和不访问的变量；
3. 消除冗余操作，去除未访问就多次重新赋值；
4. 代码强度削减，用简单操作替代复杂操作，比如移位代替乘2的幂；
5. 提取公共子表达式，多次计算用中间变量保存；
6. 循环不变代码外提，多次循环中不改变的代码可以移到循环外；
7. 函数内联，原地展开小函数，避免函数入栈出栈；
可以进一步优化吗？(3)
我认为可以死代码消除，因为内联后f1()没有被调用！
这里记录一下优化等级：
性能相关：
`-Ofast` > `-O3` > `-O2` > `-O1` > `-Og` > `-O0`(默认)
空间相关：
`-Oz` > `-Os` > `-O1` > `-O0`(默认)
## 对比编译优化的结果
这里置对比main函数内部的内容
`-O0`时
```
; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 10, ptr %2, align 4
  store i32 20, ptr %3, align 4
  %5 = load i32, ptr %2, align 4
  %6 = load i32, ptr %3, align 4
  %7 = add nsw i32 %5, %6
  store i32 %7, ptr %4, align 4
  %8 = load i32, ptr %4, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %8)
  ret i32 0
}
```
`-O1`时
```
; Function Attrs: nofree nounwind sspstrong uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef 30)
  ret i32 0
}
```
注意到应用o1后就生了两句话，`x，y，z`变量都不见了，`printf`直接调用了30。
## 对比编译优化的结果(2)
```
; Function Attrs: nofree nounwind sspstrong uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  %1 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %1)
  store volatile i32 30, ptr %1, align 4, !tbaa !5
  %2 = load volatile i32, ptr %1, align 4, !tbaa !5
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef %2)
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %1)
  ret i32 0
}
```
注意到z被强制定义了，但x，y均未出现，z被直接复制为30。
## 理解C代码与riscv指令序列的关联
这里遇到一个小bug，
```
    clang -S a.c --target=riscv64-linux-gnu
In file included from a.c:2:
In file included from /usr/include/stdio.h:440:
/usr/include/bits/floatn.h:97:9: error: __float128 is not supported on this target
   97 | typedef __float128 _Float128;
      |         ^
1 error generated.
```
这是因为索引的stdio.h中定义了float128，rv64默认是不支持128位浮点数的，需要修改标准库位置，我这里直接用了前面提到的rv64标准库位置。
```
clang -S a.c --target=riscv64-linux-gnu -I/usr/lib/gcc/riscv64-linux-gnu/15.1.0/include/ -I/usr/lib/gcc/riscv64-linux-gnu/15.1.0/include-fixed/ -I/usr/lib/gcc/riscv64-linux-gnu/15.1.0/../../../../riscv64-linux-gnu/include/ -I/usr/riscv64-linux-gnu/usr/include/
```
得到的`a.s`如下：
```
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zmmul1p0_zaamo1p0_zalrsc1p0"
	.file	"a.c"
	.text
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -48
	.cfi_def_cfa_offset 48
	sd	ra, 40(sp)                      # 8-byte Folded Spill
	sd	s0, 32(sp)                      # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	addi	s0, sp, 48
	.cfi_def_cfa s0, 0
	li	a0, 0
	sd	a0, -40(s0)                     # 8-byte Folded Spill
	sw	a0, -20(s0)
	# int x = 10;
	li	a0, 10
	sw	a0, -24(s0)
	# int y = 20;
	li	a0, 20
	sw	a0, -28(s0)
	# int z = x + y
	lw	a0, -24(s0)
	lw	a1, -28(s0)
	addw	a0, a0, a1
	sw	a0, -32(s0)
	# 后面是调用printf
	lw	a1, -32(s0)
.Lpcrel_hi0:
	auipc	a0, %pcrel_hi(.L.str)
	addi	a0, a0, %pcrel_lo(.Lpcrel_hi0)
	call	printf
                                        # kill: def $x11 killed $x10
	ld	a0, -40(s0)                     # 8-byte Folded Reload
	.cfi_def_cfa sp, 48
	ld	ra, 40(sp)                      # 8-byte Folded Reload
	ld	s0, 32(sp)                      # 8-byte Folded Reload
	.cfi_restore ra
	.cfi_restore s0
	addi	sp, sp, 48
	.cfi_def_cfa_offset 0
	ret
# 这个似乎是定义main函数
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
# 这是字符串常量
.L.str:
	.asciz	"z = %d\n"
	.size	.L.str, 8

	.ident	"clang version 20.1.8"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym printf
```

## 理解C代码与riscv指令序列的关联(2)
```
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zmmul1p0_zaamo1p0_zalrsc1p0"
	.file	"a.c"
	.text
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
.Lpcrel_hi0:
	auipc	a0, %pcrel_hi(.L.str)
	addi	a0, a0, %pcrel_lo(.Lpcrel_hi0)
	# 注意这里直接在a1寄存器写了30
	li	a1, 30
	call	printf
	li	a0, 0
	ld	ra, 8(sp)                       # 8-byte Folded Reload
	.cfi_restore ra
	addi	sp, sp, 16
	.cfi_def_cfa_offset 0
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"z = %d\n"
	.size	.L.str, 8

	.ident	"clang version 20.1.8"
	.section	".note.GNU-stack","",@progbits
	.addrsig
```
不过我使用`-Og`编译出来的结果和`-O1`是一致的。 
查看clang步骤，这是`-O1`的
```
===-------------------------------------------------------------------------===
                        Analysis execution timing report
===-------------------------------------------------------------------------===
  Total Execution Time: 0.0007 seconds (0.0007 wall clock)

   ---User Time---   --User+System--   ---Wall Time---  --- Name ---
   0.0006 ( 84.0%)   0.0006 ( 84.0%)   0.0006 ( 84.0%)  TargetIRAnalysis
   0.0000 (  2.4%)   0.0000 (  2.4%)   0.0000 (  2.4%)  AAManager
   0.0000 (  1.7%)   0.0000 (  1.7%)   0.0000 (  1.8%)  MemorySSAAnalysis
   0.0000 (  1.7%)   0.0000 (  1.7%)   0.0000 (  1.7%)  GlobalsAA
   0.0000 (  1.3%)   0.0000 (  1.3%)   0.0000 (  1.4%)  DominatorTreeAnalysis
   0.0000 (  1.4%)   0.0000 (  1.4%)   0.0000 (  1.3%)  ScalarEvolutionAnalysis
   0.0000 (  1.2%)   0.0000 (  1.2%)   0.0000 (  1.1%)  LazyCallGraphAnalysis
   0.0000 (  0.9%)   0.0000 (  0.9%)   0.0000 (  1.0%)  TargetLibraryAnalysis
   0.0000 (  0.7%)   0.0000 (  0.7%)   0.0000 (  0.7%)  LoopAnalysis
   0.0000 (  0.7%)   0.0000 (  0.7%)   0.0000 (  0.7%)  CallGraphAnalysis
   0.0000 (  0.6%)   0.0000 (  0.6%)   0.0000 (  0.6%)  InnerAnalysisManagerProxy<CGSCCAnalysisManager, Module>
   0.0000 (  0.6%)   0.0000 (  0.6%)   0.0000 (  0.5%)  OuterAnalysisManagerProxy<ModuleAnalysisManager, Function>
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.3%)  BasicAA
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.3%)  PostDominatorTreeAnalysis
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.3%)  LastRunTrackingAnalysis
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.3%)  InnerAnalysisManagerProxy<FunctionAnalysisManager, Module>
   0.0000 (  0.3%)   0.0000 (  0.3%)   0.0000 (  0.2%)  FunctionAnalysisManagerCGSCCProxy
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.2%)  ProfileSummaryAnalysis
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  AssumptionAnalysis
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  ShouldNotRunFunctionPassesAnalysis
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  LoopAccessAnalysis
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  InlineAdvisorAnalysis
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  TypeBasedAA
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  ScopedNoAliasAA
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  OuterAnalysisManagerProxy<ModuleAnalysisManager, LazyCallGraph::SCC, LazyCallGraph &>
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  OptimizationRemarkEmitterAnalysis
   0.0000 (  0.1%)   0.0000 (  0.1%)   0.0000 (  0.1%)  DemandedBitsAnalysis
   0.0007 (100.0%)   0.0007 (100.0%)   0.0007 (100.0%)  Total
```
这是`-Og`的
```
===-------------------------------------------------------------------------===
                        Analysis execution timing report
===-------------------------------------------------------------------------===
  Total Execution Time: 0.0004 seconds (0.0004 wall clock)

   ---User Time---   --System Time--   --User+System--   ---Wall Time---  --- Name ---
   0.0003 ( 87.4%)   0.0000 (  0.0%)   0.0003 ( 85.5%)   0.0003 ( 84.2%)  TargetIRAnalysis
   0.0000 (  1.0%)   0.0000 (  0.0%)   0.0000 (  1.0%)   0.0000 (  2.5%)  DominatorTreeAnalysis
   0.0000 (  2.6%)   0.0000 ( 11.1%)   0.0000 (  2.8%)   0.0000 (  2.5%)  AAManager
   0.0000 (  1.8%)   0.0000 (  0.0%)   0.0000 (  1.8%)   0.0000 (  1.6%)  GlobalsAA
   0.0000 (  0.0%)   0.0000 ( 66.7%)   0.0000 (  1.5%)   0.0000 (  1.3%)  ScalarEvolutionAnalysis
   0.0000 (  1.3%)   0.0000 (  0.0%)   0.0000 (  1.3%)   0.0000 (  1.2%)  MemorySSAAnalysis
   0.0000 (  1.0%)   0.0000 (  0.0%)   0.0000 (  1.0%)   0.0000 (  0.9%)  LazyCallGraphAnalysis
   0.0000 (  1.0%)   0.0000 (  0.0%)   0.0000 (  1.0%)   0.0000 (  0.8%)  TargetLibraryAnalysis
   0.0000 (  0.5%)   0.0000 (  0.0%)   0.0000 (  0.5%)   0.0000 (  0.7%)  LoopAnalysis
   0.0000 (  0.5%)   0.0000 ( 11.1%)   0.0000 (  0.8%)   0.0000 (  0.7%)  CallGraphAnalysis
   0.0000 (  0.5%)   0.0000 (  0.0%)   0.0000 (  0.5%)   0.0000 (  0.6%)  InnerAnalysisManagerProxy<CGSCCAnalysisManager, Module>
   0.0000 (  0.5%)   0.0000 (  0.0%)   0.0000 (  0.5%)   0.0000 (  0.5%)  OuterAnalysisManagerProxy<ModuleAnalysisManager, Function>
   0.0000 (  0.3%)   0.0000 (  0.0%)   0.0000 (  0.3%)   0.0000 (  0.4%)  BasicAA
   0.0000 (  0.3%)   0.0000 (  0.0%)   0.0000 (  0.3%)   0.0000 (  0.3%)  PostDominatorTreeAnalysis
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.2%)  LastRunTrackingAnalysis
   0.0000 (  0.3%)   0.0000 (  0.0%)   0.0000 (  0.3%)   0.0000 (  0.2%)  FunctionAnalysisManagerCGSCCProxy
   0.0000 (  0.3%)   0.0000 (  0.0%)   0.0000 (  0.3%)   0.0000 (  0.2%)  InnerAnalysisManagerProxy<FunctionAnalysisManager, Module>
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.2%)  InlineAdvisorAnalysis
   0.0000 (  0.0%)   0.0000 ( 11.1%)   0.0000 (  0.3%)   0.0000 (  0.1%)  LoopAccessAnalysis
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  ProfileSummaryAnalysis
   0.0000 (  0.3%)   0.0000 (  0.0%)   0.0000 (  0.3%)   0.0000 (  0.1%)  OuterAnalysisManagerProxy<ModuleAnalysisManager, LazyCallGraph::SCC, LazyCallGraph &>
   0.0000 (  0.3%)   0.0000 (  0.0%)   0.0000 (  0.3%)   0.0000 (  0.1%)  ScopedNoAliasAA
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  OptimizationRemarkEmitterAnalysis
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  AssumptionAnalysis
   0.0000 (  0.3%)   0.0000 (  0.0%)   0.0000 (  0.3%)   0.0000 (  0.1%)  DemandedBitsAnalysis
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  ShouldNotRunFunctionPassesAnalysis
   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.0%)   0.0000 (  0.1%)  TypeBasedAA
   0.0004 (100.0%)   0.0000 (100.0%)   0.0004 (100.0%)   0.0004 (100.0%)  Total
```
## 查看riscv64目标文件的反汇编结果
```
0000000000000000 <main>:
       0: 7179         	addi	sp, sp, -0x30
       2: f406         	sd	ra, 0x28(sp)
       4: f022         	sd	s0, 0x20(sp)
       6: 1800         	addi	s0, sp, 0x30
       8: 4501         	li	a0, 0x0
       a: fca43c23     	sd	a0, -0x28(s0)
       e: fea42623     	sw	a0, -0x14(s0)
      12: 4529         	li	a0, 0xa
      14: fea42423     	sw	a0, -0x18(s0)
      18: 4551         	li	a0, 0x14
      1a: fea42223     	sw	a0, -0x1c(s0)
      1e: fe842503     	lw	a0, -0x18(s0)
      22: fe442583     	lw	a1, -0x1c(s0)
      26: 9d2d         	addw	a0, a0, a1
      28: fea42023     	sw	a0, -0x20(s0)
      2c: fe042583     	lw	a1, -0x20(s0)

0000000000000030 <.Lpcrel_hi0>:
      30: 00000517     	auipc	a0, 0x0
      34: 00050513     	mv	a0, a0
      38: 00000097     	auipc	ra, 0x0
      3c: 000080e7     	jalr	ra <.Lpcrel_hi0+0x8>
      40: fd843503     	ld	a0, -0x28(s0)
      44: 70a2         	ld	ra, 0x28(sp)
      46: 7402         	ld	s0, 0x20(sp)
      48: 6145         	addi	sp, sp, 0x30
      4a: 8082         	ret
```
详细看一看究竟做了些什么：
这里看到rv64的指令有些是16位有些是32位的
0: 分配栈，共用了0x30字节的栈空间，sp寄存器为栈顶，当分配空间时，自减；
2: 压入ra寄存器(-0x8，以原来的栈顶为起点)，这是调用者传递的返回地址；
4: 压入s0寄存器(-0x10)，这是调用者希望保持不变的寄存器；
6: s0  =  sp+30，也就是说s0存的是原栈顶；
8: a0 = 0
a: -0x28存64位0
e: -0x14存32位0
12: a0=10
14: -0x18存32位10，对应int x=10
18: a0=20
1a: -0x1C存32位20，对应int y = 20
1e~2c:把x读到a0，把y读到a1，计算a0=a0+a1，也就是z=x+y，存z到a0，再读z到a1 
30: 这一步是将当前PC存到a0
34: 复制a0到a0？也就是NOP
38: 当前PC存到ra
3c: 跳到ra+8也就是0x40
40: a0读回了64位0，感觉这个是返回值；
44: 读回初始ra
46: 读回初始s0
48: 恢复栈顶，返回
发现里面没有printf相关内容

## 查看riscv64可执行文件的反汇编结果
发现多了很多块（函数），其中有一个`.plt` section，经查询用于调用共享库函数，注意到在main的6b8有一个jal，跳转到`.plt+0x30`，应该就是调用了`printf`。
## 对比编译优化前后的性能差异
O0:
```
0000000000001140 <main>:
    1140: 55                           	pushq	%rbp
    1141: 48 89 e5                     	movq	%rsp, %rbp
    1144: 48 83 ec 20                  	subq	$0x20, %rsp
    1148: c7 45 fc 00 00 00 00         	movl	$0x0, -0x4(%rbp)
    114f: 89 7d f8                     	movl	%edi, -0x8(%rbp)
    1152: 48 89 75 f0                  	movq	%rsi, -0x10(%rbp)
    1156: 48 c7 45 e8 00 00 00 00      	movq	$0x0, -0x18(%rbp)
    115e: 48 c7 45 e0 00 00 00 00      	movq	$0x0, -0x20(%rbp)
    1166: 48 81 7d e0 00 00 00 40      	cmpq	$0x40000000, -0x20(%rbp) # imm = 0x40000000
    116e: 7d 1a                        	jge	0x118a <main+0x4a>
    1170: 48 8b 45 e0                  	movq	-0x20(%rbp), %rax
    1174: 48 03 45 e8                  	addq	-0x18(%rbp), %rax
    1178: 48 89 45 e8                  	movq	%rax, -0x18(%rbp)
    117c: 48 8b 45 e0                  	movq	-0x20(%rbp), %rax
    1180: 48 83 c0 01                  	addq	$0x1, %rax
    1184: 48 89 45 e0                  	movq	%rax, -0x20(%rbp)
    1188: eb dc                        	jmp	0x1166 <main+0x26>
    118a: 48 8b 75 e8                  	movq	-0x18(%rbp), %rsi
    118e: 48 8d 3d 6f 0e 00 00         	leaq	0xe6f(%rip), %rdi       # 0x2004 <_IO_stdin_used+0x4>
    1195: b0 00                        	movb	$0x0, %al
    1197: e8 94 fe ff ff               	callq	0x1030 <printf@plt>
    119c: 31 c0                        	xorl	%eax, %eax
    119e: 48 83 c4 20                  	addq	$0x20, %rsp
    11a2: 5d                           	popq	%rbp
    11a3: c3                           	retq
```
O1:
```
0000000000001140 <main>:
    1140: 50                           	pushq	%rax
    1141: 48 8d 3d bc 0e 00 00         	leaq	0xebc(%rip), %rdi       # 0x2004 <_IO_stdin_used+0x4>
    1148: 48 be 00 00 00 e0 ff ff ff 07	movabsq	$0x7ffffffe0000000, %rsi # imm = 0x7FFFFFFE0000000
    1152: 31 c0                        	xorl	%eax, %eax
    1154: e8 d7 fe ff ff               	callq	0x1030 <printf@plt>
    1159: 31 c0                        	xorl	%eax, %eax
    115b: 59                           	popq	%rcx
    115c: c3                           	retq
```
O2:
```
0000000000001140 <main>:
    1140: 50                           	pushq	%rax
    1141: 48 8d 3d bc 0e 00 00         	leaq	0xebc(%rip), %rdi       # 0x2004 <_IO_stdin_used+0x4>
    1148: 48 be 00 00 00 e0 ff ff ff 07	movabsq	$0x7ffffffe0000000, %rsi # imm = 0x7FFFFFFE0000000
    1152: 31 c0                        	xorl	%eax, %eax
    1154: e8 d7 fe ff ff               	callq	0x1030 <printf@plt>
    1159: 31 c0                        	xorl	%eax, %eax
    115b: 59                           	popq	%rcx
    115c: c3                           	retq
```
发现这个程序`O1`和`O2`没有差异，实测`O0`运行时间约400ms时，`O1，O2`均只需要约3ms，有两个量级的提升！
## 程序真的从main()开始执行吗?
使用gdb运行第一条指令，用`bt`查看，发现此时frame为
```
(gdb) bt
#0  0x00007ffff7fe16c0 in _start () from /lib64/ld-linux-x86-64.so.2
#1  0x0000000000000001 in ?? ()
#2  0x00007fffffffe833 in ?? ()
#3  0x0000000000000000 in ?? ()
```
说明首先调用的是ld.so中的`_start()`函数
## 程序真的从main()返回后结束吗?
并不是，在return行设置断点，随后单步运行，发现程序没有结束，并且此时仍在`_start()`栈中：
```
(gdb) bt
#0  __libc_start_call_main (main=main@entry=0x555555555140 <main>, argc=argc@entry=1, 
    argv=argv@entry=0x7fffffffe488) at ../sysdeps/nptl/libc_start_call_main.h:74
#1  0x00007ffff7c27c4b in __libc_start_main_impl (main=0x555555555140 <main>, argc=1, 
    argv=0x7fffffffe488, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, 
    stack_end=0x7fffffffe478) at ../csu/libc-start.c:360
#2  0x0000555555555065 in _start ()
```
## 体验未指定行为
确实不一样……和讲义中的结果一样，clang汇报一个warning。
```
warning: multiple unsequenced modifications to 'i' [-Wunsequenced]
```
## 查看Linux系统中整数类型的取值范围
* char 8位 -128~127
* short -32768~32767（说明系统用的是补码表示）
* unsigned short 0~65535
* int 32位
* long 64位
* long long 64位
## 体验未定义行为
发现每次运行值是不一样的！这是因为每次传入的a0直线的内存位置中的数据发生了变化。

## PA1
运行按键测试程序后就遇到了两个bug：
1、我是用的sdl版本和PA环境不一致，`sdl2-config --cflags`会定义`_GNU_SOURCE`，导致一些文件中出现重复定义，我在这些文件中加入了
```c
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
```
解决了这个问题，紧接着我又遇到了
```
  28   │ /ysyx-workbench/abstract-machine/am/src/native/vme.c:20:23: error: field ‘hash’ has incomplete type
  29   │    20 |   struct hsearch_data hash;
  30   │       |                       ^~~~
  31   │ /ysyx-workbench/abstract-machine/am/src/native/vme.c: In function ‘protect’:
  32   │ /ysyx-workbench/abstract-machine/am/src/native/vme.c:45:13: error: implicit declaration of function ‘hcreate_r’; did you mean ‘hcreate’? [-Wimplicit-function-declaration]
  33   │    45 |   int ret = hcreate_r(max_pg, &h->hash);
  34   │       |             ^~~~~~~~~
  35   │       |             hcreate
  36   │ /ysyx-workbench/abstract-machine/am/src/native/vme.c: In function ‘map’:
  37   │ /ysyx-workbench/abstract-machine/am/src/native/vme.c:100:3: error: implicit declaration of function ‘hsearch_r’; did you mean ‘hsearch’? [-Wimplicit-function-declaration]
  38   │   100 |   hsearch_r(item, FIND, &item_find, &vm_head->hash);
  39   │       |   ^~~~~~~~~
  40   │       |   hsearch
  41   │ /ysyx-workbench/abstract-machine/am/src/native/vme.c: At top level:
  42   │ /ysyx-workbench/abstract-machine/am/src/native/vme.c:144:1: fatal error: opening dependency file /ysyx-workbench/abstract-machine/am/build/native/src/native/vme.d: No such file or directory
  43   │   144 | }
  44   │       | ^
  45   │ compilation terminated.
───────┴────────────────────────────
```
折腾了一个晚上我通过`-E -dD`检查宏发现这是被clang-format坑了，我的format规则中会对include进行排序，源代码中先`#include <search.h>`再`#include "platform.h"`，clang-format会调整成先内部库再外部库，由于`platform.h`中`#undef __USE_GNU`导致无法使用`search.h`中的GNU扩展。
我在ysyx根目录`.clang-format`添加了`SortIncludes:False`解决这个问题。
## ccache
`tldr`告诉我ccache会提供`symlinks for compilers` in `/usr/lib/ccache/bin`，所以我只需要将它添加进`PATH`，由于我是用的是fish，所以在environment.fish中加入了`fish_add_path /usr/lib/ccache/bin`
使用time来看一下时间
make：10.33s
make -j4：2.52s
make -j16：2.16s
可以看到16线程并没有变得更快，这应该是大小核的原因
make一次后
ccache make：8.50s
ccache make -j4：3.72s
ccache make -j16：4.58s
并没有比没有ccache时更快！
## 时钟功能的实现
通过`io_read(AM_TIMER_UPTIME)`获得当前运行的时间，没过1s，通过`io_read(AM_TIMER_RTC)`获得时间并打印。
## 按键功能的实现
通过`io_read(AM_INPUT_KEYBRD)`读取`ev`，这是一个结构体，包括一个布尔值keydown和一个整型keycode，随后再通过判断keycode打印。
## 显示功能的实现
将屏幕分成了N\*N的区域，然后按照逻辑更新值
