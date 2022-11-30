// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 */

#include "cam_sensor_dev.h"
#include "cam_req_mgr_dev.h"
#include "cam_sensor_soc.h"
#include "cam_sensor_core.h"
#ifdef VENDOR_EDIT
#include "linux/proc_fs.h"
#include "cam_cci_ctrl_interface.h"
struct cam_sensor_i2c_reg_setting_array {
	struct cam_sensor_i2c_reg_array reg_setting[4600];
	unsigned short size;
	enum camera_sensor_i2c_type addr_type;
	enum camera_sensor_i2c_type data_type;
	unsigned short delay;
};

struct cam_sensor_settings {
	struct cam_sensor_i2c_reg_setting_array imx586_setting;
	struct cam_sensor_i2c_reg_setting_array imx471_setting;
	struct cam_sensor_i2c_reg_setting_array imx319_setting;
	struct cam_sensor_i2c_reg_setting_array s5km5_setting;
	struct cam_sensor_i2c_reg_setting_array gc02m0b_setting;
	struct cam_sensor_i2c_reg_setting_array ov02a1b_setting;
	struct cam_sensor_i2c_reg_setting_array hi846_setting;
	struct cam_sensor_i2c_reg_setting_array s5kgd1sp_setting;
	struct cam_sensor_i2c_reg_setting_array imx616_setting;
	struct cam_sensor_i2c_reg_setting_array gc2375_setting;
	struct cam_sensor_i2c_reg_setting_array imx708_setting;
	struct cam_sensor_i2c_reg_setting_array gc02m1b_setting;
	struct cam_sensor_i2c_reg_setting_array ov64b_setting;
};

struct cam_sensor_settings sensor_settings = {
#include "CAM_SENSOR_SETTINGS.h"
};
static bool is_ftm_current_test = false;
#endif

static long cam_sensor_subdev_ioctl(struct v4l2_subdev *sd,
	unsigned int cmd, void *arg)
{
	int rc = 0;
	struct cam_sensor_ctrl_t *s_ctrl =
		v4l2_get_subdevdata(sd);
#ifdef VENDOR_EDIT
	struct cam_sensor_i2c_reg_setting sensor_setting;
#endif

	switch (cmd) {
	case VIDIOC_CAM_CONTROL:
		rc = cam_sensor_driver_cmd(s_ctrl, arg);
		break;
#ifdef VENDOR_EDIT
		case VIDIOC_CAM_FTM_POWNER_DOWN:
			CAM_ERR(CAM_SENSOR, "FTM power down");
			return cam_sensor_power_down(s_ctrl);
			break;
		case VIDIOC_CAM_FTM_POWNER_UP:
			CAM_ERR(CAM_SENSOR, "FTM power up, sensor id 0x%x", s_ctrl->sensordata->slave_info.sensor_id);
			rc = cam_sensor_power_up(s_ctrl);
			if(rc < 0) {
				CAM_ERR(CAM_SENSOR, "ftm power up failed!");
				break;
			}
			is_ftm_current_test = true;
			if (s_ctrl->sensordata->slave_info.sensor_id == 0x586 ||
				s_ctrl->sensordata->slave_info.sensor_id == 0xFFFF ||
				s_ctrl->sensordata->slave_info.sensor_id == 0xFFFE) {
				sensor_setting.reg_setting = sensor_settings.imx586_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.imx586_setting.size;
				sensor_setting.delay = sensor_settings.imx586_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET imx586 setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x471) {
				sensor_setting.reg_setting = sensor_settings.imx471_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.imx471_setting.size;
				sensor_setting.delay = sensor_settings.imx471_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET imx471 setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x319) {
				sensor_setting.reg_setting = sensor_settings.imx319_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.imx319_setting.size;
				sensor_setting.delay = sensor_settings.imx319_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET imx319 setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x30D5) {
				sensor_setting.reg_setting = sensor_settings.s5km5_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.size = sensor_settings.s5km5_setting.size;
				sensor_setting.delay = sensor_settings.s5km5_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET s5km5 setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x02d0 ||
				s_ctrl->sensordata->slave_info.sensor_id == 0xF2d0) {
				sensor_setting.reg_setting = sensor_settings.gc02m0b_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.gc02m0b_setting.size;
				sensor_setting.delay = sensor_settings.gc02m0b_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET gc02m0b setting");
			}else if(s_ctrl->sensordata->slave_info.sensor_id == 0x02e0) {
				sensor_setting.reg_setting = sensor_settings.gc02m1b_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.gc02m1b_setting.size;
				sensor_setting.delay = sensor_settings.gc02m1b_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET gc02m1b setting");
			}else if(s_ctrl->sensordata->slave_info.sensor_id == 0x25 ||
				s_ctrl->sensordata->slave_info.sensor_id == 0x2b03) {
				sensor_setting.reg_setting = sensor_settings.ov02a1b_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.ov02a1b_setting.size;
				sensor_setting.delay = sensor_settings.ov02a1b_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET ov02a1b setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x4608) {
				sensor_setting.reg_setting = sensor_settings.hi846_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.size = sensor_settings.hi846_setting.size;
				sensor_setting.delay = sensor_settings.hi846_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET HI846 setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x0841) {
				sensor_setting.reg_setting = sensor_settings.s5kgd1sp_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.size = sensor_settings.s5kgd1sp_setting.size;
				sensor_setting.delay = sensor_settings.s5kgd1sp_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET s5kgd1sp setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x0616) {
				sensor_setting.reg_setting = sensor_settings.imx616_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.imx616_setting.size;
				sensor_setting.delay = sensor_settings.imx616_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET imx616 setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x7500) {
				sensor_setting.reg_setting = sensor_settings.gc2375_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.gc2375_setting.size;
				sensor_setting.delay = sensor_settings.gc2375_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET gc2375 setting");
			} else if(s_ctrl->sensordata->slave_info.sensor_id == 0x708) {
				sensor_setting.reg_setting = sensor_settings.imx708_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.imx708_setting.size;
				sensor_setting.delay = sensor_settings.imx708_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET imx708 setting");
			}else if(s_ctrl->sensordata->slave_info.sensor_id == 0x5664) {
				sensor_setting.reg_setting = sensor_settings.ov64b_setting.reg_setting;
				sensor_setting.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE;
				sensor_setting.size = sensor_settings.ov64b_setting.size;
				sensor_setting.delay = sensor_settings.ov64b_setting.delay;
				CAM_ERR(CAM_SENSOR,"FTM GET ov64b setting");
			}
			rc = camera_io_dev_write(&(s_ctrl->io_master_info), &sensor_setting);

			if (rc < 0) {
				CAM_ERR(CAM_SENSOR, "FTM Failed to write sensor setting");
			} else {
				CAM_ERR(CAM_SENSOR, "FTM successfully to write sensor setting");
			}
			break;
#endif

	default:
		CAM_ERR(CAM_SENSOR, "Invalid ioctl cmd: %d", cmd);
		rc = -EINVAL;
		break;
	}
	return rc;
}

static int cam_sensor_subdev_close(struct v4l2_subdev *sd,
	struct v4l2_subdev_fh *fh)
{
	struct cam_sensor_ctrl_t *s_ctrl =
		v4l2_get_subdevdata(sd);

	if (!s_ctrl) {
		CAM_ERR(CAM_SENSOR, "s_ctrl ptr is NULL");
		return -EINVAL;
	}

	mutex_lock(&(s_ctrl->cam_sensor_mutex));
	#ifdef VENDOR_EDIT
	if(!is_ftm_current_test)
	#endif
	cam_sensor_shutdown(s_ctrl);
	mutex_unlock(&(s_ctrl->cam_sensor_mutex));

	return 0;
}

#ifdef CONFIG_COMPAT
static long cam_sensor_init_subdev_do_ioctl(struct v4l2_subdev *sd,
	unsigned int cmd, unsigned long arg)
{
	struct cam_control cmd_data;
	int32_t rc = 0;

	if (copy_from_user(&cmd_data, (void __user *)arg,
		sizeof(cmd_data))) {
		CAM_ERR(CAM_SENSOR, "Failed to copy from user_ptr=%pK size=%zu",
			(void __user *)arg, sizeof(cmd_data));
		return -EFAULT;
	}

	switch (cmd) {
	case VIDIOC_CAM_CONTROL:
		rc = cam_sensor_subdev_ioctl(sd, cmd, &cmd_data);
		if (rc < 0)
			CAM_ERR(CAM_SENSOR, "cam_sensor_subdev_ioctl failed");
			break;
	default:
		CAM_ERR(CAM_SENSOR, "Invalid compat ioctl cmd_type: %d", cmd);
		rc = -EINVAL;
	}

	if (!rc) {
		if (copy_to_user((void __user *)arg, &cmd_data,
			sizeof(cmd_data))) {
			CAM_ERR(CAM_SENSOR,
				"Failed to copy to user_ptr=%pK size=%zu",
				(void __user *)arg, sizeof(cmd_data));
			rc = -EFAULT;
		}
	}

	return rc;
}

#endif
static struct v4l2_subdev_core_ops cam_sensor_subdev_core_ops = {
	.ioctl = cam_sensor_subdev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl32 = cam_sensor_init_subdev_do_ioctl,
#endif
	.s_power = cam_sensor_power,
};

static struct v4l2_subdev_ops cam_sensor_subdev_ops = {
	.core = &cam_sensor_subdev_core_ops,
};

static const struct v4l2_subdev_internal_ops cam_sensor_internal_ops = {
	.close = cam_sensor_subdev_close,
};

static int cam_sensor_init_subdev_params(struct cam_sensor_ctrl_t *s_ctrl)
{
	int rc = 0;

	s_ctrl->v4l2_dev_str.internal_ops =
		&cam_sensor_internal_ops;
	s_ctrl->v4l2_dev_str.ops =
		&cam_sensor_subdev_ops;
	strlcpy(s_ctrl->device_name, CAMX_SENSOR_DEV_NAME,
		sizeof(s_ctrl->device_name));
	s_ctrl->v4l2_dev_str.name =
		s_ctrl->device_name;
	s_ctrl->v4l2_dev_str.sd_flags =
		(V4L2_SUBDEV_FL_HAS_DEVNODE | V4L2_SUBDEV_FL_HAS_EVENTS);
	s_ctrl->v4l2_dev_str.ent_function =
		CAM_SENSOR_DEVICE_TYPE;
	s_ctrl->v4l2_dev_str.token = s_ctrl;

	rc = cam_register_subdev(&(s_ctrl->v4l2_dev_str));
	if (rc)
		CAM_ERR(CAM_SENSOR, "Fail with cam_register_subdev rc: %d", rc);

	return rc;
}

static int32_t cam_sensor_driver_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int32_t rc = 0;
	int i = 0;
	struct cam_sensor_ctrl_t *s_ctrl = NULL;
	struct cam_hw_soc_info   *soc_info = NULL;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		CAM_ERR(CAM_SENSOR,
			"%s :i2c_check_functionality failed", client->name);
		return -EFAULT;
	}

	/* Create sensor control structure */
	s_ctrl = kzalloc(sizeof(*s_ctrl), GFP_KERNEL);
	if (!s_ctrl)
		return -ENOMEM;

	i2c_set_clientdata(client, s_ctrl);

	s_ctrl->io_master_info.client = client;
	soc_info = &s_ctrl->soc_info;
	soc_info->dev = &client->dev;
	soc_info->dev_name = client->name;

	/* Initialize sensor device type */
	s_ctrl->of_node = client->dev.of_node;
	s_ctrl->io_master_info.master_type = I2C_MASTER;
	s_ctrl->is_probe_succeed = 0;
	s_ctrl->last_flush_req = 0;

	rc = cam_sensor_parse_dt(s_ctrl);
	if (rc < 0) {
		CAM_ERR(CAM_SENSOR, "cam_sensor_parse_dt rc %d", rc);
		goto free_s_ctrl;
	}

	rc = cam_sensor_init_subdev_params(s_ctrl);
	if (rc)
		goto free_s_ctrl;

	s_ctrl->i2c_data.per_frame =
		kzalloc(sizeof(struct i2c_settings_array) *
		MAX_PER_FRAME_ARRAY, GFP_KERNEL);
	if (s_ctrl->i2c_data.per_frame == NULL) {
		rc = -ENOMEM;
		goto unreg_subdev;
	}

	INIT_LIST_HEAD(&(s_ctrl->i2c_data.init_settings.list_head));
	INIT_LIST_HEAD(&(s_ctrl->i2c_data.config_settings.list_head));
	INIT_LIST_HEAD(&(s_ctrl->i2c_data.streamon_settings.list_head));
	INIT_LIST_HEAD(&(s_ctrl->i2c_data.streamoff_settings.list_head));

	for (i = 0; i < MAX_PER_FRAME_ARRAY; i++)
		INIT_LIST_HEAD(&(s_ctrl->i2c_data.per_frame[i].list_head));

	s_ctrl->bridge_intf.device_hdl = -1;
	s_ctrl->bridge_intf.link_hdl = -1;
	s_ctrl->bridge_intf.ops.get_dev_info = cam_sensor_publish_dev_info;
	s_ctrl->bridge_intf.ops.link_setup = cam_sensor_establish_link;
	s_ctrl->bridge_intf.ops.apply_req = cam_sensor_apply_request;
	s_ctrl->bridge_intf.ops.flush_req = cam_sensor_flush_request;

	s_ctrl->sensordata->power_info.dev = soc_info->dev;

	return rc;
unreg_subdev:
	cam_unregister_subdev(&(s_ctrl->v4l2_dev_str));
free_s_ctrl:
	kfree(s_ctrl);
	return rc;
}

static int cam_sensor_platform_remove(struct platform_device *pdev)
{
	int                        i;
	struct cam_sensor_ctrl_t  *s_ctrl;
	struct cam_hw_soc_info    *soc_info;

	s_ctrl = platform_get_drvdata(pdev);
	if (!s_ctrl) {
		CAM_ERR(CAM_SENSOR, "sensor device is NULL");
		return 0;
	}

	CAM_INFO(CAM_SENSOR, "platform remove invoked");
	mutex_lock(&(s_ctrl->cam_sensor_mutex));
	cam_sensor_shutdown(s_ctrl);
	mutex_unlock(&(s_ctrl->cam_sensor_mutex));
	cam_unregister_subdev(&(s_ctrl->v4l2_dev_str));
	soc_info = &s_ctrl->soc_info;
	for (i = 0; i < soc_info->num_clk; i++)
		devm_clk_put(soc_info->dev, soc_info->clk[i]);

	kfree(s_ctrl->i2c_data.per_frame);
	platform_set_drvdata(pdev, NULL);
	v4l2_set_subdevdata(&(s_ctrl->v4l2_dev_str.sd), NULL);
	devm_kfree(&pdev->dev, s_ctrl);

	return 0;
}

static int cam_sensor_driver_i2c_remove(struct i2c_client *client)
{
	int                        i;
	struct cam_sensor_ctrl_t  *s_ctrl = i2c_get_clientdata(client);
	struct cam_hw_soc_info    *soc_info;

	if (!s_ctrl) {
		CAM_ERR(CAM_SENSOR, "sensor device is NULL");
		return 0;
	}

	CAM_INFO(CAM_SENSOR, "i2c remove invoked");
	mutex_lock(&(s_ctrl->cam_sensor_mutex));
	cam_sensor_shutdown(s_ctrl);
	mutex_unlock(&(s_ctrl->cam_sensor_mutex));
	cam_unregister_subdev(&(s_ctrl->v4l2_dev_str));
	soc_info = &s_ctrl->soc_info;
	for (i = 0; i < soc_info->num_clk; i++)
		devm_clk_put(soc_info->dev, soc_info->clk[i]);

	kfree(s_ctrl->i2c_data.per_frame);
	v4l2_set_subdevdata(&(s_ctrl->v4l2_dev_str.sd), NULL);
	kfree(s_ctrl);

	return 0;
}

static const struct of_device_id cam_sensor_driver_dt_match[] = {
	{.compatible = "qcom,cam-sensor"},
	{}
};

#ifdef VENDOR_EDIT
#define COMMAND_SIZE 255

static ssize_t ois_read(struct file *p_file,
	char __user *puser_buf, size_t count, loff_t *p_offset)
{
    return 1;
}

static ssize_t ois_write(struct file *p_file,
	const char __user *puser_buf,
	size_t count, loff_t *p_offset)
{
	char data[COMMAND_SIZE] = {0};
	char* const delim = " ";
	int iIndex = 0;
	char *token = NULL, *cur = NULL;
	uint32_t addr =0, value = 0;
//	int result = 0;
	unsigned char  read_data = 0;
	struct camera_cci_transfer ccit;

	if(puser_buf) {
		if (copy_from_user(&data, puser_buf, count)){
			CAM_ERR(CAM_OIS, "copy_from_user fail");
		}
	}

	cur = data;
	while ((token = strsep(&cur, delim))) {
		//CAM_ERR(CAM_OIS, "string = %s iIndex = %d, count = %d", token, iIndex, count);
		if (iIndex  == 0) {
			if (kstrtoint(token, 16, &addr)){
				CAM_ERR(CAM_OIS, "kstrtoint fail");
				return -1;
			}
		} else if (iIndex == 1) {
		    if (kstrtoint(token, 16, &value)){
				CAM_ERR(CAM_OIS, "kstrtoint fail");
				return -1;
			}
		}
		iIndex++;
	}

	memset(&ccit,0,sizeof(ccit));
	ccit.cmd = CAMERA_CCI_INIT;
	cam_cci_control_interface(&ccit);


    memset(&ccit,0,sizeof(ccit));
    ccit.cmd = CAMERA_CCI_READ;
    ccit.addr = addr;
    ccit.data = &read_data;
    ccit.count = 1;
    cam_cci_control_interface(&ccit);

	memset(&ccit,0,sizeof(ccit));
	ccit.cmd = CAMERA_CCI_RELEASE;
	cam_cci_control_interface(&ccit);

	CAM_ERR(CAM_OIS, "read addr = 0x%x, value = 0x%x ", addr, read_data);

#if 0
	if (ois_ctrls[CAM_OIS_MASTER] && addr != 0) {
		result = RamWrite32A_oneplus(ois_ctrls[CAM_OIS_MASTER], addr, value);
		if (result < 0) {
			CAM_ERR(CAM_OIS, "write addr = 0x%x, value = 0x%x fail", addr, value);
		} else {
			CAM_INFO(CAM_OIS, "write addr = 0x%x, value = 0x%x success", addr, value);
		}
	}
#endif
	return count;
}



static const struct file_operations proc_file_fops = {
	.owner = THIS_MODULE,
	.read  = ois_read,
	.write = ois_write,
};

#endif
static int32_t cam_sensor_driver_platform_probe(
	struct platform_device *pdev)
{
	int32_t rc = 0, i = 0;
	struct cam_sensor_ctrl_t *s_ctrl = NULL;
	struct cam_hw_soc_info *soc_info = NULL;

	/* Create sensor control structure */
	s_ctrl = devm_kzalloc(&pdev->dev,
		sizeof(struct cam_sensor_ctrl_t), GFP_KERNEL);
	if (!s_ctrl)
		return -ENOMEM;

	soc_info = &s_ctrl->soc_info;
	soc_info->pdev = pdev;
	soc_info->dev = &pdev->dev;
	soc_info->dev_name = pdev->name;

	/* Initialize sensor device type */
	s_ctrl->of_node = pdev->dev.of_node;
	s_ctrl->is_probe_succeed = 0;
	s_ctrl->last_flush_req = 0;

	/*fill in platform device*/
	s_ctrl->pdev = pdev;

	s_ctrl->io_master_info.master_type = CCI_MASTER;

	rc = cam_sensor_parse_dt(s_ctrl);
	if (rc < 0) {
		CAM_ERR(CAM_SENSOR, "failed: cam_sensor_parse_dt rc %d", rc);
		goto free_s_ctrl;
	}

	/* Fill platform device id*/
	pdev->id = soc_info->index;

	rc = cam_sensor_init_subdev_params(s_ctrl);
	if (rc)
		goto free_s_ctrl;

	s_ctrl->i2c_data.per_frame =
		kzalloc(sizeof(struct i2c_settings_array) *
		MAX_PER_FRAME_ARRAY, GFP_KERNEL);
	if (s_ctrl->i2c_data.per_frame == NULL) {
		rc = -ENOMEM;
		goto unreg_subdev;
	}

	INIT_LIST_HEAD(&(s_ctrl->i2c_data.init_settings.list_head));
	INIT_LIST_HEAD(&(s_ctrl->i2c_data.config_settings.list_head));
	INIT_LIST_HEAD(&(s_ctrl->i2c_data.streamon_settings.list_head));
	INIT_LIST_HEAD(&(s_ctrl->i2c_data.streamoff_settings.list_head));

	for (i = 0; i < MAX_PER_FRAME_ARRAY; i++)
		INIT_LIST_HEAD(&(s_ctrl->i2c_data.per_frame[i].list_head));

	s_ctrl->bridge_intf.device_hdl = -1;
	s_ctrl->bridge_intf.link_hdl = -1;
	s_ctrl->bridge_intf.ops.get_dev_info = cam_sensor_publish_dev_info;
	s_ctrl->bridge_intf.ops.link_setup = cam_sensor_establish_link;
	s_ctrl->bridge_intf.ops.apply_req = cam_sensor_apply_request;
	s_ctrl->bridge_intf.ops.flush_req = cam_sensor_flush_request;

	s_ctrl->sensordata->power_info.dev = &pdev->dev;
	platform_set_drvdata(pdev, s_ctrl);
	s_ctrl->sensor_state = CAM_SENSOR_INIT;

	return rc;
unreg_subdev:
	cam_unregister_subdev(&(s_ctrl->v4l2_dev_str));
free_s_ctrl:
	devm_kfree(&pdev->dev, s_ctrl);
	return rc;
}

MODULE_DEVICE_TABLE(of, cam_sensor_driver_dt_match);

static struct platform_driver cam_sensor_platform_driver = {
	.probe = cam_sensor_driver_platform_probe,
	.driver = {
		.name = "qcom,camera",
		.owner = THIS_MODULE,
		.of_match_table = cam_sensor_driver_dt_match,
		.suppress_bind_attrs = true,
	},
	.remove = cam_sensor_platform_remove,
};

static const struct i2c_device_id i2c_id[] = {
	{SENSOR_DRIVER_I2C, (kernel_ulong_t)NULL},
	{ }
};

static struct i2c_driver cam_sensor_driver_i2c = {
	.id_table = i2c_id,
	.probe = cam_sensor_driver_i2c_probe,
	.remove = cam_sensor_driver_i2c_remove,
	.driver = {
		.name = SENSOR_DRIVER_I2C,
	},
};

static int __init cam_sensor_driver_init(void)
{
	int32_t rc = 0;
#ifdef VENDOR_EDIT
	struct proc_dir_entry *face_common_dir = NULL;
	struct proc_dir_entry *proc_file_entry = NULL;
#endif
	rc = platform_driver_register(&cam_sensor_platform_driver);
	if (rc < 0) {
		CAM_ERR(CAM_SENSOR, "platform_driver_register Failed: rc = %d",
			rc);
		return rc;
	}

	rc = i2c_add_driver(&cam_sensor_driver_i2c);
	if (rc)
		CAM_ERR(CAM_SENSOR, "i2c_add_driver failed rc = %d", rc);
#ifdef VENDOR_EDIT
	//Create OIS control node
	face_common_dir =  proc_mkdir("cam_sensor", NULL);
	if(!face_common_dir) {
		CAM_ERR(CAM_OIS, "create dir fail CAM_ERROR API");
		//return FACE_ERROR_GENERAL;
	}

	proc_file_entry = proc_create("SensorControl", 0777, face_common_dir, &proc_file_fops);
	if(proc_file_entry == NULL) {
		CAM_ERR(CAM_OIS, "Create fail");
	} else {
		CAM_INFO(CAM_OIS, "Create successs");
}

#endif
	return rc;
}

static void __exit cam_sensor_driver_exit(void)
{
	platform_driver_unregister(&cam_sensor_platform_driver);
	i2c_del_driver(&cam_sensor_driver_i2c);
}

module_init(cam_sensor_driver_init);
module_exit(cam_sensor_driver_exit);
MODULE_DESCRIPTION("cam_sensor_driver");
MODULE_LICENSE("GPL v2");