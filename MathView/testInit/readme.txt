测试D3D11初始化过程
参考文档：http://blog.csdn.net/bonchoix/article/details/8288868

主要步骤：

		1. 创建设备ID3D11Device和设备上下文ID3D11DeviceContext；			完成
		2. 检测多重采样支持的等级：CheckMultisampleQualityLevels			完成		
		3. 创建交换链														完成
		4. 创建RenderTargetView
		5. 创建DepthStencilView
		6. 把上述两个视图绑定到渲染管线相应的阶段
		7. 设置Viewport