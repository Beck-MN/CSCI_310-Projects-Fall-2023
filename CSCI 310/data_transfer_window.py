import PySimpleGUI as sg

def data_transfer_window(window1):
    
    layout = [
    [sg.Text('Select a file to open')],
    [sg.InputText(), sg.FileBrowse(key='-FILE-')],
    [sg.Button('Open'), sg.Button('Exit')]
]

    data_window = sg.Window('Transfer Data', layout, size=(
    1200, 800), element_justification='c')

    while True:
        event, values = data_window.read()
        if event in (sg.WIN_CLOSED, 'Exit'):
            break
        elif event == 'Open':
            file_path = values['-FILE-']

    window1.un_hide()
    data_window.close()
