class Boggle:
    """
    Represents a Boggle game board and dictionary.
    """

    def __init__(self, grid, dictionary):
        """Create a Boggle game with a grid and dictionary."""
        self.grid = grid
        self.dictionary = dictionary
        self.solution = []

    def setGrid(self, grid):
        """Set the Boggle grid."""
        self.grid = grid

    def setDictionary(self, dictionary):
        """Set the dictionary of words."""
        self.dictionary = dictionary

    def getSolution(self):
        """
        Find all dictionary words that can be created on the board.

        Returns an empty list if the grid or dictionary is invalid.
        """
        self.solution = []

        # Check that the grid is valid.
        if not self.validGrid():
            return []

        # Check that the dictionary is valid.
        if not isinstance(self.dictionary, list):
            return []

        # Search for every word in the dictionary.
        for word in self.dictionary:

            # Every dictionary word must be a string.
            if not isinstance(word, str):
                continue

            # Words must contain at least 3 letters.
            if len(word) < 3:
                continue

            # Search for the word.
            if self.searchWord(word):
                self.solution.append(word)

        return self.solution

    def validGrid(self):
        """Check whether the Boggle grid is valid."""

        # Grid must be a list and cannot be empty.
        if not isinstance(self.grid, list):
            return False

        if len(self.grid) == 0:
            return False

        # Every row must be a list.
        for row in self.grid:
            if not isinstance(row, list):
                return False

            # Every row must contain at least one tile.
            if len(row) == 0:
                return False

        # Every row must have the same number of columns.
        columns = len(self.grid[0])

        for row in self.grid:
            if len(row) != columns:
                return False

        # Every tile must be a non-empty string.
        for row in self.grid:
            for tile in row:
                if not isinstance(tile, str):
                    return False

                if len(tile) == 0:
                    return False

        return True

    def searchWord(self, word):
        """
        Search the entire board for a specific word.

        Returns True if the word can be created and False otherwise.
        """

        # Convert the word to uppercase for comparison.
        word = word.upper()

        # Create a visited board.
        visited = []

        for row in self.grid:
            visited.append([False] * len(row))

        # Try starting the word from every tile.
        for row in range(len(self.grid)):
            for col in range(len(self.grid[row])):

                if self.search(word, 0, row, col, visited):
                    return True

        return False

    def search(self, word, index, row, col, visited):
        """
        Recursively search for a word starting at a particular tile.

        index = position in the word we are currently trying to match
        row   = current row
        col   = current column
        visited = tiles already used in this path
        """

        # Get the current tile.
        tile = self.grid[row][col].upper()

        # Do not use a tile that has already been used.
        if visited[row][col]:
            return False

        # Determine how many letters this tile represents.
        tile_length = len(tile)

        # Check whether the tile matches the current portion of the word.
        if word[index:index + tile_length] != tile:
            return False

        # Mark this tile as used.
        visited[row][col] = True

        # Move forward in the word by the number of letters
        # contained in the tile.
        new_index = index + tile_length

        # If we have used the entire word, we found it.
        if new_index == len(word):
            visited[row][col] = False
            return True

        # The tile cannot match if it goes beyond the end of the word.
        if new_index > len(word):
            visited[row][col] = False
            return False

        # All 8 possible directions.
        directions = [
            (-1, -1), (-1, 0), (-1, 1),
            (0, -1),           (0, 1),
            (1, -1),  (1, 0),  (1, 1)
        ]

        # Try every neighboring tile.
        for row_change, col_change in directions:

            new_row = row + row_change
            new_col = col + col_change

            # Make sure the new row is inside the grid.
            if new_row < 0 or new_row >= len(self.grid):
                continue

            # Make sure the new column is inside the grid.
            if new_col < 0 or new_col >= len(self.grid[new_row]):
                continue

            # Do not use a tile that has already been used.
            if visited[new_row][new_col]:
                continue

            # Recursively search from the neighboring tile.
            if self.search(
                word,
                new_index,
                new_row,
                new_col,
                visited
            ):
                visited[row][col] = False
                return True

        # Backtrack:
        # We are finished with this tile, so allow it to be used
        # in a different possible path.
        visited[row][col] = False

        return False


def main():
    """
    Test the Boggle solver using the example from the assignment.
    """

    grid = [
        ["A", "B", "C", "D"],
        ["E", "F", "G", "H"],
        ["IE", "J", "K", "L"],
        ["A", "B", "C", "D"]
    ]

    dictionary = [
        "ABEF",
        "AFJIEEB",
        "DGKD",
        "DGKA"
    ]

    mygame = Boggle(grid, dictionary)

    print(mygame.getSolution())


if __name__ == "__main__":
    main()