import asyncio
import sys
import time

from bleak import BleakScanner, BleakClient

TARGET_NAME = "nrf_bulbulator"
UINT16_UUID = '00005678-0000-1000-8000-00805f9b34fb'
UINT8_UUID = '00006660-0000-1000-8000-00805f9b34fb'

NrfValuesDict = {
    "uint16_": UINT16_UUID,
    "uint8_": UINT8_UUID
}


async def get_values(address):
    async with BleakClient(address) as client:

        for value_uuid in NrfValuesDict.values():
            try:
                value = await client.read_gatt_char(value_uuid)
                print(f"Value under {value_uuid[:9]}: {value}")
            except Exception as ex:
                print(ex)


async def find_nrf():
    while True:
        devices = await BleakScanner.discover()
        print("Loading devices:")
        for d in devices:
            print(d)
            if d.name == TARGET_NAME:
                return d
        time.sleep(5)

    return None


async def main():
    nrf_device = await find_nrf()
    if nrf_device is not None:
        print("Connection established")
        await get_values(nrf_device.address)
    else:
        sys.exit("No device found")


asyncio.run(main())
