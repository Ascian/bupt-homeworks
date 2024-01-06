import { Image } from "@chakra-ui/react";

export default function UserIcon({ boxSize = '40px' }) {
    return (
        <Image
            borderRadius="full"
            boxSize={boxSize}
            src="/userIcon.png"
            alt="User Icon"
        />);
}