void kernel()
{
    volatile unsigned char* video_memory = (volatile unsigned char*)0xb8000;
    video_memory[0] = 'X';
    video_memory[1] = 0x2f;

    while (1)
    {
        // Code
    }
}