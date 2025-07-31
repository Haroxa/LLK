# LLK项目介绍

## 项目概述
本项目是一个基于Visual Studio开发的连连看游戏（LLK），实现了连连看游戏的核心玩法、用户管理、排行榜和游戏设置等功能。

## 功能模块
- **游戏核心逻辑**：CGameLogic.cpp/.h实现连连看游戏算法
- **界面交互**：CGameDlg.cpp/.h、LLKDlg.cpp/.h负责游戏界面和用户交互
- **用户管理**：UserManagement.cpp/.h处理用户登录、注册和数据管理
- **排行榜功能**：CGameRankDlg.cpp/.h实现游戏分数排行
- **游戏设置**：CGameSetting.cpp/.h提供游戏参数配置

## 目录结构
- **根目录**：包含Visual Studio解决方案文件(LLK.sln)和项目配置
- **LLK/**：主项目代码目录
  - **数据结构**：游戏核心算法实现
  - **界面组件**：对话框和UI元素实现
  - **用户数据**：data/目录存储用户信息和临时地图
  - **资源文件**：res/目录包含图标和位图资源
  - **主题文件**：theme/目录包含游戏主题资源

## 开发环境
- Visual Studio (支持x64平台)
- Windows操作系统

## 使用方法
1. 用Visual Studio打开LLK.sln
2. 编译生成项目
3. 运行LLK.exe可执行文件
4. 首次使用需注册用户，登录后即可开始游戏

## 注意事项
- 项目包含.gitignore文件，排除编译产物和临时文件
- 用户数据存储在data/users.txt
- 临时地图文件位于data/tempMap/