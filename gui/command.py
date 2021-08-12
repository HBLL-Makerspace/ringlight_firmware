SET_CHANNEL_RGBI_ID = 0x02
SET_CHANNEL_W_ID = 0x03

class Command:
    def __init__(self, id, cmd, data):
        self.id = id
        self.cmd = cmd
        self.data = data

    def set_id(self, id):
        self.id = id

    def set_cmd(self, cmd):
        self.cmd = cmd

    def set_data(self, data):
        self.data = data

    def command_to_binary(self):
        cmd = [0xff, self.id, self.cmd]
        for i in self.data:
            cmd.append(i)
        cmd.append(0x00);
        cmd.append(0x00);
        return cmd


class CmdSetChannelRGB(Command):
    def __init__(self, id, channel, r, g, b):
        super().__init__(id, SET_CHANNEL_RGBI_ID, [channel, r, g, b])

class CmdSetChannelW(Command):
    def __init__(self, id, channel, w):
        super().__init__(id, SET_CHANNEL_W_ID, [channel, w])
