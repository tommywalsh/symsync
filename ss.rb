#!/usr/bin/ruby

require 'ss_ui.rb'
require 'Qt'


class SyncCheckBox < Qt::CheckBox

  slots 'onStateChanged(int)', 'onPressed()'

  def initialize(startState, parent = nil)
    super(parent)
    setTristate(true)
    setCheckState(startState)
    connect(self, SIGNAL('pressed()'), self, SLOT('onPressed()'))
  end

  def onPressed
    print 'pressed'
  end

  def onStateChanged(newState)
    
    if newState == Qt::Unchecked
      print 'no sync'
    else
      print 'sync'
    end
  end
end


class SyncFileLine < Qt::Widget
  NonExistant = 0
  Directory = 1
  SoftLinkedDir = 2
  SoftLinkedFile = 3

  def initialize (syncFileType, fileName, indentLevel, parent = nil)
    super(parent)

    if syncFileType == NonExistant
      check = Qt::Unchecked
    elsif syncFileType == Directory
      check = Qt::PartiallyChecked
    else
      check = Qt::Checked
    end

    box = SyncCheckBox.new(check, self)
    label = Qt::Label.new(fileName, self)

    layout = Qt::HBoxLayout.new(self)
#    layout = Qt::HBoxLayout.new
    layout.addWidget(box)
    layout.addWidget(label)

#    self.setLayout(layout)
    
  end

end



class SyncGui < Qt::Widget
  def initialize(parent = nil)
    super(parent)
    setFixedSize(200, 120)

#    box = SyncCheckBox.new(Qt::Checked, self);
    line = SyncFileLine.new(SyncFileLine::Directory, 'Hoodoo Gurus', 0, self) 

    quit = Qt::PushButton.new('Quit', self)
    quit.setGeometry(62, 40, 75, 30)
    quit.setFont(Qt::Font.new('Times', 18, Qt::Font::Bold))

    connect(quit, SIGNAL('clicked()'), $qApp, SLOT('quit()'))
  end

end



app = Qt::Application.new(ARGV)

widget = SyncGui.new()
widget.show()

app.exec()
