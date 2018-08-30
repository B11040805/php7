/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2017 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Authors: Dmitry Stogov <dmitry@zend.com>                             |
   +----------------------------------------------------------------------+
*/

// PHP 将内存分为3中大小
// 为什么还会出现内存出现限制的问题
// 如果程序报内存不够用，第一反应是内存泄漏，啥是内存泄漏呢，就是占用内存，没有释放，大的IO操作可能出现这类的问题，比如，MySQL查询返回的大量数据，一次大文件的读取等
// 程序操作大数据时产生拷贝，php 是写时复制（Copy On Write），也就是说，当新变量被赋值时内存不发生变化，直到新变量的内容被操作时才会产生复制。
// 配置不合理系统资源不够用，限制post的大小
// php-fpm 内存泄露问题
// 在一台常见的Nginx+php-fpm的服务器上 1、Nginx服务器fork出n个子进程，php-fpm管理器fork出n个子进程。
// 当有请求，Nginx的一个worker接收请求，并将请求跑到socket中。
// php-fpm空闲的子进程监听到socket的请求，接收并处理。
// 一个php-fpm的生命周期大致是这样的，模块初始化-》请求初始化-》请求处理-》请求结束-》模块关闭
// 在请求初始化（RINIT）-> 请求处理 -> 请求结束（RSHUTDOWN）这个“请求处理”过程是： php 读取相应的 php 文件，对其进行词法分析，生成 opcode ， zend 虚拟机执行 opcode 。
// php 在每次请求结束后自动释放内存，有效避免了常见场景下内存泄露的问题，然而实际环境中因某些扩展的内存管理没有做好或者 php 代码中出现循环引用导致未能正常释放不用的资源。
// 效果好，好到多少
// php python java 语言的内存回收机制都是一样的，那有没有有的语言是不使用内存回收的，rust语言，而 Rust 通过新加入的 生命周期 (lifetime)和所有权 (Ownership) 特性，就可以实现内存的自动回收，并且无需运行时， 可以大大地避免出现类似内存泄漏或者时悬停指针之类的问题
// 据说，在 Rust 的编译器设计理念里，编译器可以检查的，就无需程序员操心。并且因为 Rust 所倡导的安全的概念，所以它的编译器会做尽可能多的检查，争取在编译期就发现问 题。此外 Rust 的编译信息还非常友好和详细
#ifndef ZEND_ALLOC_SIZES_H
#define ZEND_ALLOC_SIZES_H

#define ZEND_MM_CHUNK_SIZE (2 * 1024 * 1024)               /* 2 MB  */
#define ZEND_MM_PAGE_SIZE  (4 * 1024)                      /* 4 KB  */
#define ZEND_MM_PAGES      (ZEND_MM_CHUNK_SIZE / ZEND_MM_PAGE_SIZE)  /* 512 */
#define ZEND_MM_FIRST_PAGE (1)

#define ZEND_MM_MIN_SMALL_SIZE		8
#define ZEND_MM_MAX_SMALL_SIZE      3072
#define ZEND_MM_MAX_LARGE_SIZE      (ZEND_MM_CHUNK_SIZE - (ZEND_MM_PAGE_SIZE * ZEND_MM_FIRST_PAGE))

/* num, size, count, pages */
#define ZEND_MM_BINS_INFO(_, x, y) \
	_( 0,    8,  512, 1, x, y) \
	_( 1,   16,  256, 1, x, y) \
	_( 2,   24,  170, 1, x, y) \
	_( 3,   32,  128, 1, x, y) \
	_( 4,   40,  102, 1, x, y) \
	_( 5,   48,   85, 1, x, y) \
	_( 6,   56,   73, 1, x, y) \
	_( 7,   64,   64, 1, x, y) \
	_( 8,   80,   51, 1, x, y) \
	_( 9,   96,   42, 1, x, y) \
	_(10,  112,   36, 1, x, y) \
	_(11,  128,   32, 1, x, y) \
	_(12,  160,   25, 1, x, y) \
	_(13,  192,   21, 1, x, y) \
	_(14,  224,   18, 1, x, y) \
	_(15,  256,   16, 1, x, y) \
	_(16,  320,   64, 5, x, y) \
	_(17,  384,   32, 3, x, y) \
	_(18,  448,    9, 1, x, y) \
	_(19,  512,    8, 1, x, y) \
	_(20,  640,   32, 5, x, y) \
	_(21,  768,   16, 3, x, y) \
	_(22,  896,    9, 2, x, y) \
	_(23, 1024,    8, 2, x, y) \
	_(24, 1280,   16, 5, x, y) \
	_(25, 1536,    8, 3, x, y) \
	_(26, 1792,   16, 7, x, y) \
	_(27, 2048,    8, 4, x, y) \
	_(28, 2560,    8, 5, x, y) \
	_(29, 3072,    4, 3, x, y)

#endif /* ZEND_ALLOC_SIZES_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
