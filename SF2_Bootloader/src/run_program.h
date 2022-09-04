#ifndef __RUN_PROGRAM_H__
#define __RUN_PROGRAM_H__


/* function to copy code to eSRAM*/
void copy_image_to_esram();

/* function to remap eSRAM to cortex - M3 code region*/
void remap_user_code_eSRAM_0(void);

void run_iap(void);

void run_application(void);

#endif /* __RUN_PROGRAM_H__ */
