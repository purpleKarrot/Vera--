-- Keywords break and continue should be immediately followed by a semicolon

for file in vera.input_files() do
    local tokens = vera.get_tokens(file, 1, 0, -1, -1, {"break", "continue"})
    for _, token in ipairs(tokens) do
        keyword = token.name
        line = token.line
        column = token.column
        semicolons = vera.get_tokens(file, line, column + string.len(keyword), line + 1, 0, {"semicolon"})
        semicolon = semicolons[1]
        if not semicolon or semicolon.column ~= column + string.len(keyword) then
            vera.report(file, line, "keyword '"..keyword.."' not immediately followed by a semicolon")
        end
    end
end
