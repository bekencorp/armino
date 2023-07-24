for %%f in (*.jpg) do (
	del %%~nf_output_rgb565.rgb
	..\bin\ffmpeg -i %%f -pix_fmt rgb565le %%~nf_output_rgb565.rgb
)

pause