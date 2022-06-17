local pd <const> = playdate

function _addMenuFromLua(name, callback)
    local menu = playdate.getSystemMenu()
    local menuItem, error = menu:addMenuItem(name, callback)
end
