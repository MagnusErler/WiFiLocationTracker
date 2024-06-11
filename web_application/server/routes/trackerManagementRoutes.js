const express = require("express");
const axios = require("axios");
const path = require("path");
const fs = require("fs");
const https = require("https");

const router = express.Router();

// Helper functions
const readCredentials = (account) => {
  const cert_file = path.join(__dirname, "../credentials", `${account}.crt`);
  const key_file = path.join(__dirname, "../credentials", `${account}.key`);
  const trust_file = path.join(__dirname, "../credentials", `${account}.trust`);

  return {
    cert: fs.readFileSync(cert_file),
    key: fs.readFileSync(key_file),
    ca: fs.readFileSync(trust_file),
  };
};

const handleAxiosError = (error, res) => {
  console.error("Request error:", error);
  let errorMessage = "Request error";
  if (axios.isAxiosError(error)) {
    errorMessage = "Failed to connect to server";
  } else if (
    error.response &&
    error.response.data &&
    error.response.data.message
  ) {
    errorMessage = error.response.data.message;
  } else {
    errorMessage = error.message;
  }
  res
    .status(error.response ? error.response.status : 500)
    .json({ error: errorMessage });
};

router.get("/getDeviceInfoFromJoinserver", async (req, res) => {
  try {
    const a = "acct-d13";
    const h = "de02lnxprodjs01.loraclouddemo.com:7009";
    const u = "/api/v1/appo/list_devices";
    const url = `https://${h}${u}`;
    const credentials = readCredentials(a);

    const response = await axios.get(url, {
      httpsAgent: new https.Agent(credentials),
    });

    res.status(200).json(response.data);
  } catch (error) {
    console.error("Failed to fetch device information:", error);
    res.status(500).json({ error: "Failed to fetch device information" });
  }
});

router.delete("/unclaimDeviceFromModemServices", async (req, res) => {
  console.log("Unclaiming device from Modem Services:", req.body);
  try {
    let { Token, deveuis } = req.body;

    if (!Token || !deveuis || !Array.isArray(deveuis) || deveuis.length === 0) {
      return res.status(400).json({
        error: "Bad request",
        message:
          "API token and a non-empty array of deveuis are required in the request body",
      });
    }

    deveuis = deveuis.map((deveui) =>
      deveui.includes("-") ? deveui : deveui.match(/.{1,2}/g).join("-")
    );

    const response = await axios.post(
      "https://mgs.loracloud.com/api/v1/device/remove",
      { deveuis },
      {
        headers: {
          Authorization: Token,
          "Content-Type": "application/json",
        },
      }
    );

    res.status(200).json(response.data);
  } catch (error) {
    console.error("Failed to remove devices:", error);
    const errorMessage = axios.isAxiosError(error)
      ? "Failed to connect to server"
      : error.message;
    res.status(500).json({ error: "Request error", message: errorMessage });
  }
});

router.post("/claimDevicesOnJoinServer", async (req, res) => {
  if (!Array.isArray(req.body)) {
    return res.status(400).json({
      error: "Bad request",
      message: "Request body must be a non-empty array of devices",
    });
  }

  try {
    const a = "acct-d13";
    const h = "de02lnxprodjs01.loraclouddemo.com:7009";
    const u = "/api/v1/appo/claim_devices";
    const url = `https://${h}${u}`;
    const credentials = readCredentials(a);

    const devices = req.body.map((device) => ({
      DevEUI: device.DevEUI,
      ChipEUI: device.ChipEUI || device.DevEUI,
      JoinEUI: device.JoinEUI,
      claim: device.Pin,
    }));

    console.log("Devices to claim on LoRaCloud:", devices);

    const response = await axios.post(url, devices, {
      httpsAgent: new https.Agent(credentials),
      headers: { "Content-Type": "application/json" },
    });

    if (!Array.isArray(response.data)) {
      console.error(
        "Unexpected response format from server. All devices failed to claim."
      );
      return res.status(500).json({
        error: "Server error",
        message:
          "Unexpected response format from server. All devices failed to claim.",
      });
    }

    const failedDevices = response.data.filter((device) => device.error);
    if (failedDevices.length > 0) {
      const errorMessages = failedDevices
        .map((device) => device.error)
        .join(", ");
      if (
        errorMessages.includes("[IsYours] Device is already claimed by you")
      ) {
        return res
          .status(409)
          .json({
            error: "Device claim conflict",
            message: `Failed to claim device(s): ${errorMessages}`,
          });
      }
      if (errorMessages.includes("[BadClaim] Claim does not verify")) {
        return res
          .status(400)
          .json({
            error: "Bad request",
            message: `Failed to claim device(s): ${errorMessages}`,
          });
      }
      if (
        errorMessages.includes(
          "[UnknownEUI] ChipEUI is not provisioned on this server"
        )
      ) {
        return res
          .status(404)
          .json({
            error: "Device not found",
            message: `Failed to claim device(s): ${errorMessages}`,
          });
      }
      return res
        .status(500)
        .json({
          error: "Server error",
          message: `Failed to claim device(s): ${errorMessages}`,
        });
    }

    res
      .status(200)
      .json({
        success: true,
        message: "Devices claimed successfully",
        data: response.data,
      });
  } catch (error) {
    console.error("Failed to claim devices:", error);
    const errorMessage = axios.isAxiosError(error)
      ? "Failed to connect to server"
      : error.message;
    res.status(500).json({ error: "Request error", message: errorMessage });
  }
});

router.delete("/api/unclaimDeviceOnJoinServer", async (req, res) => {
  if (!Array.isArray(req.body)) {
    return res.status(400).json({
      error: "Bad request",
      message: "Request body must be a non-empty array of devices",
    });
  }

  try {
    const account = "acct-d13";
    const url = `https://de02lnxprodjs01.loraclouddemo.com:7009/api/v1/appo/unclaim_devices`;
    const credentials = readCredentials(account);

    const devices = req.body.map((device) => ({ DevEUI: device.DevEUI }));
    console.log("Devices to unclaim from LoRaCloud:", devices);

    const response = await axios.post(url, devices, {
      httpsAgent: new https.Agent(credentials),
      headers: { "Content-Type": "application/json" },
    });

    res
      .status(200)
      .json({ message: "Devices unclaimed successfully", data: response.data });
  } catch (error) {
    handleAxiosError(error, res);
  }
});

router.post("/api/createDeviceOnTTNIDServer", async (req, res) => {
  const { Token, AppID, deviceID, devEUI, joinEUI } = req.body;

  if (!Token || !AppID || !deviceID || !devEUI || !joinEUI) {
    return res
      .status(400)
      .json({
        error: "Device ID, DevEUI, JoinEUI, App ID and API token are required",
      });
  }

  try {
    const url = `https://eu1.cloud.thethings.network/api/v3/applications/${AppID}/devices`;
    const requestData = {
      end_device: {
        ids: { device_id: deviceID, dev_eui: devEUI, join_eui: joinEUI },
        join_server_address: "eu1.cloud.thethings.network",
        network_server_address: "eu1.cloud.thethings.network",
        application_server_address: "eu1.cloud.thethings.network",
      },
      field_mask: {
        paths: [
          "join_server_address",
          "network_server_address",
          "application_server_address",
          "ids.dev_eui",
          "ids.join_eui",
        ],
      },
    };

    const response = await axios.post(url, requestData, {
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${Token}`,
      },
    });

    res.json(response.data);
  } catch (error) {
    handleAxiosError(error, res);
  }
});

router.put("/api/createDeviceOnTTNJoinServer", async (req, res) => {
  const { Token, AppID, deviceID, devEUI, joinEUI } = req.body;

  if (!Token || !AppID || !deviceID || !devEUI || !joinEUI) {
    return res
      .status(400)
      .json({
        error: "Device ID, DevEUI, JoinEUI, App ID and API token are required",
      });
  }

  try {
    const url = `https://eu1.cloud.thethings.network/api/v3/js/applications/${AppID}/devices/${deviceID}`;
    const requestData = {
      end_device: {
        ids: { device_id: deviceID, dev_eui: devEUI, join_eui: joinEUI },
        network_server_address: "eu1.cloud.thethings.network",
        application_server_address: "eu1.cloud.thethings.network",
      },
      field_mask: {
        paths: [
          "network_server_address",
          "application_server_address",
          "ids.dev_eui",
          "ids.join_eui",
        ],
      },
    };

    const response = await axios.put(url, requestData, {
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${Token}`,
      },
    });

    res.json(response.data);
  } catch (error) {
    handleAxiosError(error, res);
  }
});

router.put("/api/createDeviceOnTTNNS", async (req, res) => {
  const { Token, AppID, deviceID, devEUI, joinEUI } = req.body;

  if (!Token || !AppID || !deviceID || !devEUI || !joinEUI) {
    return res
      .status(400)
      .json({
        error: "Device ID, DevEUI, JoinEUI, App ID and API token are required",
      });
  }

  try {
    const url = `https://eu1.cloud.thethings.network/api/v3/ns/applications/${AppID}/devices/${deviceID}`;
    const requestData = {
      end_device: {
        ids: { device_id: deviceID, dev_eui: devEUI, join_eui: joinEUI },
      },
      field_mask: { paths: ["ids.device_id", "ids.dev_eui", "ids.join_eui"] },
    };

    const response = await axios.put(url, requestData, {
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${Token}`,
      },
    });

    res.json(response.data);
  } catch (error) {
    handleAxiosError(error, res);
  }
});

router.delete("/api/deleteDeviceOnTTNNS", async (req, res) => {
  const { Token, AppID, DeviceID } = req.body;

  if (!AppID || !DeviceID || !Token) {
    return res
      .status(400)
      .json({ error: "App ID, Device ID, and API token are required" });
  }

  try {
    const url = `https://eu1.cloud.thethings.network/api/v3/ns/applications/${AppID}/devices/${DeviceID}`;
    const response = await axios.delete(url, {
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${Token}`,
      },
    });

    res.json(response.data);
  } catch (error) {
    handleAxiosError(error, res);
  }
});

router.delete("/api/deleteDeviceOnTTNAS", async (req, res) => {
  const { Token, AppID, DeviceID } = req.body;

  if (!AppID || !DeviceID || !Token) {
    return res
      .status(400)
      .json({ error: "App ID, Device ID, and API token are required" });
  }

  try {
    const url = `https://eu1.cloud.thethings.network/api/v3/as/applications/${AppID}/devices/${DeviceID}`;
    const response = await axios.delete(url, {
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${Token}`,
      },
    });

    res.json(response.data);
  } catch (error) {
    handleAxiosError(error, res);
  }
});

router.delete("/api/deleteDeviceOnTTN", async (req, res) => {
  const { Token, AppID, DeviceID } = req.body;

  if (!AppID || !DeviceID || !Token) {
    return res
      .status(400)
      .json({ error: "App ID, Device ID, and API token are required" });
  }

  try {
    const url = `https://eu1.cloud.thethings.network/api/v3/applications/${AppID}/devices/${DeviceID}`;
    const response = await axios.delete(url, {
      headers: {
        "Content-Type": "application/json",
        Authorization: `Bearer ${Token}`,
      },
    });

    res.json(response.data);
  } catch (error) {
    handleAxiosError(error, res);
  }
});

module.exports = router;